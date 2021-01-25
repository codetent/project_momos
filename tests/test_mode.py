from momos.components.mode import FailureModeResolver, FailureMode, UnboundFailureMode, failure_mode

from pyshould import should, should_not


def test_decorator():
    class T(FailureModeResolver):
        @failure_mode
        def failing(self):
            """foobar"""
            return []

    T.failing | should.be_an_instance_of(UnboundFailureMode)

    t = T()
    t.failing | should.be_an_instance_of(FailureMode)
    t.failing.results | should.be_empty()
    t.failing.description | should.be_equal('foobar')
