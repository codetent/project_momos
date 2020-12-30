#include "testif.hpp"


template<typename R, typename ...A>
class MessageBuilder {
    private:
        R (*fn)(A...);

    public:
        MessageBuilder(R (*fn)(A...))
        {
            this->fn = fn;
        }

        template<typename D>
        SimulatedMessage<D> call(A... args) 
        {
            TestInterface<D>& interface = TestInterface<D>::get_instance();

            this->fn(args...);

            SimulatedMessage<D> msg = interface.tx_channel.messages.back();
            interface.tx_channel.messages.pop_back();

            return msg;
        }
};
