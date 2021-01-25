from pyshould import should, should_not

from momos.graph import StateGraph
from momos.parser.parse import parse


def test_parse_invalid():
    graph = parse('''
        a = 5
    ''')

    graph | should.be_instance_of(StateGraph)
    graph.states | should.have_length(0)
    graph.transitions | should.have_length(0)


def test_parse_basic():
    graph = parse('''
        // @state A
        // @state B
        // @transition A -> B
    ''')

    graph | should.be_instance_of(StateGraph)

    graph.states | should.have_length(2)
    graph.states[0].id | should.be_equal('A')
    graph.states[1].id | should.be_equal('B')

    graph.transitions | should.have_length(1)
    graph.transitions[0].from_state.id | should.be_equal('A')
    graph.transitions[0].to_state.id | should.be_equal('B')
    graph.transitions[0].trigger.id | should.be_none()
