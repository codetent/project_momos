#include <assert.h>
#include <iostream>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "steps.hpp"
#include "testif.h"


using namespace std;


class TestStep {
    public:
        string name;
        vector<int> args;
        step_function_t resolved;

        static TestStep build(const YAML::Node &node)
        {
            TestStep step;
            step.name = node["name"].as<string>();
            step.args = node["args"].as<vector<int>>();
            step.resolved = step_mapping[step.name];

            assert(step.resolved);
            return step;
        }

        void run()
        {
            cout << "    " << "- " << this->name << ":" << endl;
            this->resolved(this->args);
        }
};


class StateTestCase {
    private:
        TestInterface &interface = TestInterface::get_instance();

    public:
        string title;
        list<TestStep> steps;

        static StateTestCase build(const YAML::Node &node)
        {
            StateTestCase test_case;
            test_case.title = node["title"].as<string>();

            YAML::const_iterator it;
            for(it=node["steps"].begin(); it!=node["steps"].end(); it++)
            {
                test_case.steps.push_back(TestStep::build(*it));
            }

            return test_case;
        }

        void run()
        {
            cout << "- " << this->title << endl;

            interface.reset();

            list<TestStep>::iterator it;
            for (it=this->steps.begin(); it!=this->steps.end(); it++) {
                it->run();
            }
        }
};


class StateTestSuite {
    public:
        string title;
        list<StateTestCase> cases;

        static StateTestSuite build(const YAML::Node &node)
        {
            StateTestSuite suite;
            suite.title = node["title"].as<string>();

            YAML::const_iterator it;
            for (it=node["cases"].begin(); it!=node["cases"].end(); it++)
            {
                suite.cases.push_back(StateTestCase::build(*it));
            }

            return suite;
        }

        static StateTestSuite load(const string &path)
        {
            return StateTestSuite::build(YAML::LoadFile(path));
        }

        void run()
        {
            cout << "[" << this->title << "]" << endl;

            list<StateTestCase>::iterator it;
            for (it=this->cases.begin(); it!=this->cases.end(); it++) {
                it->run();
            }
        }
};


int main(int argc, char **argv)
{
    StateTestSuite test_suite = StateTestSuite::load("config.yaml");
    TestInterface &test_interface = TestInterface::get_instance();
    
    test_suite.run();
    return 0;
}