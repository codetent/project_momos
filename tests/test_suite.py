from pyshould import should, should_not

from momos.components import State, Transition
from momos.graph import StateGraph
from momos.suite import TestSuite, get_simple_paths, get_transition
from momos.utils import ExternalElement

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
        graph.add_state(state)

    for transition in transitions.values():
        graph.add_transition(transition)

    return graph, states, transitions


def test_get_transition():
    graph, states, transitions = _get_test_graph()

    get_transition(graph, states['init'], states['middle']) | should.be_equal(transitions[('init', 'middle')])


def test_get_simple_paths():
    graph, _, transitions = _get_test_graph()

    paths = get_simple_paths(graph)
    paths | should.have_length(3)
    paths | should.contain_the_item(
        [transitions[('init', 'middle')], transitions[('middle', 'last')], transitions[('last', 'init')]])
    paths | should.contain_the_item(
        [transitions[('init', 'other')], transitions[('other', 'last')], transitions[('last', 'init')]])
    paths | should.contain_the_item([transitions[('init', 'other')], transitions[('other', 'free')]])


def test_suite_of():
    graph, _, _ = _get_test_graph()
    suite = TestSuite.of(graph, state_var=ExternalElement.of('state'), data_type=ExternalElement.of('int'))

    suite.cases | should_not.be_empty()
