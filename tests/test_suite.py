from pyshould import should, should_not

from momos.components import State, Transition
from momos.graph import StateGraph
from momos.suite import TestSuite

TestSuite.__test__ = False  # disable pytest collection


def _get_test_graph():
    graph = StateGraph()
    states = {
        'init': State('init', True),
        'middle': State('middle'),
        'other': State('other'),
        'last': State('last'),
        'free': State('free'),
    }
    transitions = {
        ('init', 'middle'): Transition(states['init'], states['middle']),
        ('middle', 'last'): Transition(states['middle'], states['last']),
        ('last', 'init'): Transition(states['last'], states['init']),
        ('init', 'other'): Transition(states['init'], states['other']),
        ('other', 'last'): Transition(states['other'], states['last']),
        ('other', 'free'): Transition(states['other'], states['free']),
    }

    for state in states.values():
        graph.states.append(state)

    for transition in transitions.values():
        graph.transitions.append(transition)

    return graph, states, transitions


def test_get_transition():
    graph, states, transitions = _get_test_graph()

    graph.get_transition(states['init'], states['middle']) | should.be_equal(transitions[('init', 'middle')])


def test_get_simple_paths():
    graph, _, transitions = _get_test_graph()

    paths = graph.simple_edge_paths
    paths | should.have_length(6)
    paths | should.contain_the_item([transitions[('init', 'middle')]])
    paths | should.contain_the_item([transitions[('init', 'middle')], transitions[('middle', 'last')]])
    paths | should.contain_the_item(
        [transitions[('init', 'middle')], transitions[('middle', 'last')], transitions[('last', 'init')]])
    paths | should.contain_the_item([transitions[('init', 'other')]])
    paths | should.contain_the_item([transitions[('init', 'other')], transitions[('other', 'last')]])
    paths | should.contain_the_item([transitions[('init', 'other')], transitions[('other', 'free')]])
    #  init -> other -> last -> init: ignored, because last -> init is already tested


def test_suite_of():
    graph, _, _ = _get_test_graph()
    suite = TestSuite.of(graph)

    suite.cases | should_not.be_empty()


def test_suite_attrs():
    graph, _, _ = _get_test_graph()
    suite = TestSuite.of(graph)

    suite.cases[0].description | should.be_a_string()
    suite.cases[0].priority | should.be_greater_than(0)

    suite.cases[0].steps[0].description | should.be_a_string()
    suite.cases[0].steps[0].fails | should.be_a_boolean()
    suite.cases[0].steps[0].arguments | should.be_a_list()
