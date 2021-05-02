/**
 * Convert snakecase string to camelcase.
 * @param {string} str Snakecase string
 * @returns Camelcase string
 */
function snakeToCamel(str) {
    return str.replace(
        /([-_][a-z])/g,
        (group) => group.toUpperCase()
            .replace('-', '')
            .replace('_', ''));
}


new Vue({
    el: '#q-app',
    data: function () {
        return {
            state: {
                loadingReport: false,
                showingSelector: true,
            },
            report: null,
            dot: null,
            testCases: [],
            testCaseOrder: {
                selected: 'success',
                available: ['success', 'priority'],
            },
            successCount: 0,
        };
    },
    methods: {
        /**
         * Colorize rendered dot graph regarding passed & failing transitions.
         * @param {string} id Identifier of test case
         */
        colorizeGraph: function (id) {
            const testCase = this.testCases.find((testCase) => testCase.id == id);
            const edges = this.$graph.selectAll('svg .edge')
                .filter((d, _) => !d.key.startsWith('__initial__->')).nodes();

            for (const edge of edges) {
                const title = d3.select(edge).select('title').text();
                const step = testCase.steps.find((step) => step.fromState + '->' + step.toState == title);

                edge.classList.remove('failing');
                edge.classList.remove('success');

                if (step) {
                    edge.classList.add(step.success ? 'success' : 'failing');
                }
            }
        },
    },
    computed: {
        /**
         * Same as testCases but sorted by the set order key.
         */
        sortedTestCases: function () {
            const orderKey = this.testCaseOrder.selected;

            return this.testCases.sort((a, b) => {
                return a[orderKey] - b[orderKey];
            });
        }
    },
    watch: {
        /**
         * Render dot graph if new dot string is set.
         */
        dot: function (val, oldVal) {
            this.$graph = d3.select('#graph');
            this.$graph.graphviz()
                .zoom(false)
                .fit(true)
                .renderDot(val);
        },

        /**
         * Read set file and process content.
         */
        report: function (val, oldVal) {
            const reader = new FileReader();

            this.state.loadingReport = true;

            this.testCases = [];

            reader.onload = () => {
                const parser = new DOMParser();
                const content = parser.parseFromString(reader.result, 'text/xml');

                const testSuites = content.querySelector('testsuites');
                const testCases = content.querySelectorAll('testcase');

                if (!testSuites) {
                    this.state.loadingReport = false;
                    this.$q.notify({
                        type: 'negative',
                        message: 'Invalid file format.',
                    });
                    return;
                }

                if (!testSuites.hasAttribute('momos')) {
                    this.state.loadingReport = false;
                    this.$q.notify({
                        type: 'negative',
                        message: 'Invalid momos test report.',
                    });
                    return;
                }

                this.dot = testSuites.getAttribute('momos-state_graph').replace('{', '{ rankdir="LR"');
                this.successCount = testCases.length;

                for (const testCase of testCases) {
                    const details = {
                        success: true,
                    };

                    for (const prop of testCase.querySelectorAll(`property[name^="momos-"]`)) {
                        const path = snakeToCamel(prop.getAttribute('name').substring(6).replaceAll('-', '.'));
                        let value = prop.getAttribute('value');

                        try {
                            value = JSON.parse(value);
                        } catch { }

                        objectPath.set(details, path, value);
                    }

                    for (const step of details.steps) {
                        if (!step.success) {
                            details.success = false;
                            this.successCount -= 1;
                        }
                    }

                    this.testCases.push(details);

                    this.state.loadingReport = false;
                    this.state.showingSelector = false;
                }
            };
            reader.readAsText(val);
        }
    }
});
