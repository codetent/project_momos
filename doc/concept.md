

# Introduction

## Motivation

## Protocols

In order that interlocutors can communicate with each other, they must agree on a common language. In the context of electronic systems, when computer systems are communicating, these languages are called protocols. They care about initialising connections and the actual exchange of the information. [Q1]

### Layer Model

A computer system consists of several abstraction layers to make them viable for a variety of use cases and for separating the communication logic into smaller units. Each of these layers has its own protocol.

A layer of a protocol stack (collection of all layers) of a device exchanges messages with the same layer of a different device if the flow is observed from a logical view. Physically, a message traverses all layers in the stack by passing data to the lower layer on the sender. The receiver runs this process in reverse: the data is passed from the lowest to the highest layer. 

The multilayer message flow can be illustrated using the "two philosophers analogy" [Q3]. Two philosophers (highest layer - "application") want to discuss a topic. Unfortunately, they do not have a common language and therefore they have to delegate the communication to another party. Therefore, translators (middle layer - "communication") are hired. The translators are no experts in the discussion topic but they know how they can understand each other. The translators are very busy translating so they contact secretaries (lowest layer - "transportation") who actually transmit or receive the messages. If now a philosopher wants to communicate, he tells the message its translator that rewrites it to a neutral language which is then transmitted by the secretary. On the other side, the process is reversed and the secretary passes the message to the translator who converts the neutral language to the native language understood by its philosopher. [Q3] Considered from an outer perspective, it looks like the philosophers are communicating themselves.

From a technical view, messages increase their size when they are processed by deeper layers. Each layer adds its own protocol specific header to give the remote side meta information about a received packet. Using this information, a protocol knows if the message is delivered with multiple packets or which portion it should pass to the higher layer (payload).

| Philosophers analogy [Q3]        | Technical overview [Q3]    |
|:--------------------------------:|:--------------------------:|
| ![I1](./images/philosophers.svg) | ![I2](./images/layers.svg) |

This layer model comes with a few important aspects [Q2]:

- The logical message flow takes place horizontally (peer-to-peer) without depending on upper or lower layer. The other protocols are only defining the payload of a message.
- The message payload is irrelevant for the functioning of the corresponding layer. Only the header is processed. This leads to an increase of the packet size from top to bottom on the sender side, while the size at the receiver side decreases when it is processed by more and more layers.
- Less overhead for the highest layers because they only have to care about the actual content. The other details regarding the safe transmission are abstracted away by the lower layers.
- Issues occured at lower layers are not affecting the higher layer. This problems are usually detected at the corresponding lower layers and the defect message is not passed further. In fact, it can be even possible that the highest layers do not even recognize a problem with the transportation.

### Protocol Development Process

Protocols are usually implemented in software. Therefore, the development process follows the process of software development. But there are also some differences when protocols are developed. 

In general, the process can be splitted in to several development phases. There are different models which can be used ranging from strict waterfall models to agile methods. Since protocols are usually critical software and can be found in embedded systems too, the waterfall model is often preferred for protocols.

The development phase starts with requirement plannung for the protocol that should be created.
The following design process is dependent on the actual usage case. Either the service is defined first or the focus only lies on the protocol itself. The most protocols used for the internet or in embedded networks are mostly skipping the service definition step.

After designing the protocol, the design has to be checked against its performance and functional correctness. The first is important to ensure the compliance with realtime or memory specifications. This is usually done using analystic models or by creating prototypes. Afterwards, the correctness regarding all properties of the design is checked. This is usually done using model checkers like [UPPAAL](https://uppaal.org/). These two steps are very important for finding and fixing problems as early in the process as possible, because the later it is found the higher the repairment costs.

The design process shall not contain any implementation specific decisions. Rather it should be held as generic as possible. Therefore, the implementation of the protocol has to be planned before the actual coding work. Often, this stage is accompanied by intermediate verification steps like reviews or other measures to reduce the failure rate.

When the protocol is implemented, several test steps follow to check the actual implementation. This protocol test consists of different white and black box testing approaches like unit tests or system tests. Afterwards, an additional compliance test checks the system against the requirements by a black box test which is confirmed by issuing a certification by a test laboratory.

The tested protocol is then integrated into a system or another product which represents also the last stage of checking the interoperability. In this stage, differences compared to similar implementations can be found. Additionally, the robustness or performance can be tested at the end of the development cycle.

### Protocol Structure

### Protocols in the Automotive Industry

TODO: why protocol = state machine

### Example: gPTP

## State Machines

A state machine describes the behavior of a model. This concept focuses on finite state machines which are state machines containing only a finite number of reachable states.
In short, the states are known in advance and the overall amount does not change over time.

As state machines can be represented as graphs, the paper focuses only on the graphical representation called stated graphs. Such graphs consists of nodes and edges called states and transitions in this context. All the transitions connect two states with a top-down direction. Therefore, the graph is called directed graph. For simplifying the graphs, all transitions are only unidirectional. If a state shall be connected with its predecessor, a new transition have to be created. The advantage of using the graph form is that graph theory can be applied. This is a mathemathical subject covers many algorithms and formulas. It especially gained attentation in the last years in the sector of machine learning where nearly all agorithms are based on this theory. 

The connections between the states, called transitions, are coupled with an action. If the specified action occurs, one state transitions into another one. If there is no action, the system the current state remains the same. This demonstrates the storing capability of a state machine. It is also possible that a transition ends into the same state it is arisen from. This pattern is called cycle and is present in the most state machines. Cycles are problematic because they make it hard to predict the exact behavior especially regarding timing.

A collection of states and their transitions ordered by there occurence is called path. Paths describe the traversable ways through the state machine. If there are cycles, the count of all paths is infinite because it is possible that a feedback transition is traversed multiple times. A simple state machine is a special case without any loops which contains a finite amount of paths. As cycles are a common element in normal state machines, the processing is done using simple paths. This special type of path does not contain any loop and is therefore much easier to process. Additionally, omitting the loops does not change the behavior of the state graph any further because a loop in a path is only executing some actions multiple times.

## Graph Representations

State graphs can be created using various graphical tools or code based:

Graphical tools are especially usefull for creating state graphs from scratch or easily documenting previously existing state machines.

Tools like Lucidchard or draw.io are mainly intended for quickly visualizing graphical forms or structures without mainly focusing on state machines. They are usually equipped with toolboxes containing elementary forms for different diagram types like UML or simple flow charts. In general, the visualizations are exported to common image formats for placing them manually in documentations.

Specialized tooling for state machines also exists like YAKINDU state chart tools or JFLAP. This software provides beside the visualization features for editing state machines and checking for their functionality. Additionally, these tools ship with a simulator which allows running the state machine inside the suite.

Code based approaches are typically used for creating graphics in various formats or processing them further for any other purpose. The advantage compared to graphical tools is that this process can be automated. Additionally, the output is not depending on the user of a tool, but rather it is deterministic. In general, all tools for creating graphs can be utilized for this application.

One of the most famous visual programming languages is the Unified Modelling Language (UML) based on XML. It provides various graph types for documenting software components, their composition or algorithms. UML state charts are a diagram type especially created for finite state machines. These charts are focusing on an event-driven software flow: it waits for an event, dispatches it and after consuming it, it waits again for the next event. It visualized directed graphs using states, called nodes, and transition arrows denoted by connectors. In addition, two other elements are available: guards and actions. Transitions with guard conditions can only be traversed if the expressions evaluates to true. Actions are the reaction to an event like calling functions or changing to another state. [\[1\]](https://en.wikipedia.org/wiki/UML_state_machine)

The DOT language was created for visualizing structural information as abstract graphs. It is not based on any existing language like UML, but rather it comes with its own domain specific language. Usually, these graphs are automatically generated by documentation tools like doxygen. The advantage over UML is that the DSL is simpler and shorter than writing graphs in XML. So they can even be written manually as code without any further effort. DOT supports various graph models like directed and unidrected graphs and is not only limited to state machines. Additionally, the styling can be changed by modifying node or edge attributes. Different tools like Graphviz or various libraries for many programming languages implement this language and allow generting such rendered graphs.

There are also other visual languages like DOT but especially designed for state machines. State Machine cat is one if them. In general, it is a simplified version of DOT without depending on Graphviz tools. The code primarily declares transitions and the states are automatically generated. Compared to DOT the user must not define graph types or the structure typically used for state graphs because this is done by the language itself. [\[3\]](https://github.com/sverweij/state-machine-cat)

## Protocol Specification

## Protocol Implementation Testing

## Test Case Selection

## Code Documentation

Documentiation is crucial for developing understandable and maintainable software. In the most cases, good documentation is very rare and even some companies rely on the code only instead of having an explicit documentation unter the motto "our code is our documentation". But in short: "every engineer is also a writer" [\[4\]](https://developers.google.com/tech-writing) because a product or software component is useless if they target audience does not know how to use it.

The required documents depend on the product and the targeted audience. A software component shall be shipped with an user guide describing the main concepts and how it can be integrated in another project. References are representing the existing interface and all the inputs and the resulting outputs. If the product is a user application, the reference is usually ommitted and the user guide contains the descriptions of the software.

When developing software, the main focus is on the implementation work. Writing guides is in the most cases not preferred by developers and therefore not much time is spent on it. Some companies are employing technical writers but as they are not the creator of a product, their insight or technical knowledge is limited.

To lower the barriers for developers getting a good documentation, the state-of-the-art way is generating one from existing documents, for example source code. Generation is usually done through adding annotations (mostly using comments) to the implementation which are then parsed and converted to a human-readable format like a PDF document. Additionally, modern tools support creating graphs of the components that are integrated into the result. If the documentation contains examples, which is highly preferred, these can also be automatically checked against their proper functioning.

## State of the Art

# Concept

This paper describes a method for generating test cases automatically for testing protocol implementations based on finite state machines. For this the approach uses code annotations of the implementation using comments. The target of this annotations is providing a machine-readable format while keeping them readable for other developers. In general, these comments shall integrate nicely into the source code without disclosing that they are parsed by software.

For generating the test cases, the code annotations are extracted and analyzed. Afterwards, a state graph is built using the information gained through the annotations. The graph is then split into its flow paths. Each of the state transitions of a path is coupled with a predefined trigger which lists all possible failure modes for its type. Using this failure mode collection of all transitions, a test case is generated for each failure mode and path. Additionally, test cases are created for checking a non-failing path for checking if the code annotations are correct. After generating the test suite, all test cases are analyzed and the ones most likely to fail are selected. This threshold can be set by the tester.

Since there are already some popular test runner tools like Google Test, this method is designed to work perfectly together with this existing tooling. It allows the selection of a predefined test code flavor and additional custom ones can be added by the user. The approach does not run any test by itself. For this, a test runner is required and must be setup by the user. The only available interface to the test case generator tool is a developed command line interface which allows specifying parameters for customizing the oputput. Tests written in C or any other compiled language require compiling before they can be executed. For this case, the generation tool can be called manually before compiling the test code or some tools like cmake also allow executing of a program before the compilation is started.

If it is the case that the protocol implementation requires transition triggers not covered with the ones shipped with the generation tool. Custom triggers shall be supported by defining them via a Python plugin system.

Generating test cases is not the only target of this approach. It also simplifies documenting the protocol implementation. For this, the state graph build by the annotations can be converted to an image which can be included in the final code documentation. If there is no extra documentation, the annotations itself will also help developers to understand the implemented state machine.

## System Overview

## Graph Definition

The state machine is defined using comments in the source code file. The content of this comments is called annotations because it consists of metainformation parsed by the generation tool. These comments must be added by the developer to the implementation and should serve als general code documentation too. Therefore, the comments must be readable by a software and humans.

There are different formats for specifing graphs like state machines as plain text. Several types were analyzed for this paper. The overall problem concerning all these languages is that they are not specifially created for such an application. They are mainly focused on a readable export format for other tools or as human-readable method for creating simple graphs. Of course, they all can be utilized for this purpose but then there will be some problems:

If the state machine is written in languages like DOT, the user has to write rather illogicial expressions, easy understandable if you remember that you are describing an image and perfectly suited for machines. But, you will not write this comments if you are just describing a state machine implementation. This totally affects the readability of the approach. Adding this annotations should be as simple and intuitiv as possible, because if the hurdle is too high for developers, creating this comments will be neglected and the method will not be utilized for the project.

If it requires more comments to describe the state machine compared to the lines of code of the actual implementation, the source code file will be polluted with comments and developers will mainly spend their time to write these annotations. Defining a graph with common languages requires beside of the definition of the states, a list of all the transitions. This specification has to be a single comment, because the language then takes this single block and converts it to the graph. Therefore, this languages are not the perfect tool for source code annotation because adding additional comments where the states are defined or the transitions are made is much more intuitive for the developers.

As the graph languages are designed for describing nodes and edges only. This is perfectly fine defining states and transitions of a state graph but this information is too less for creating test cases out of it. Transitions must be coupled with a trigger to set when the condition when it transitions to another state. Besides, there must be the option to add additional information about a transition like the time value for timed transitions. Common graph languages do not provide a way to set this per default. Instead this values can be set as additional styling options or as the label of the element. This requires an additional syntax (or language) which specifies the format and which have to be parsed.

Because of all these drawbacks, and there is no way around creating some custom format, an own domain specific language is created especially for this application.

### Custom Domain Specific Language

## Graph Structure

## Test Structure

## Flow

## Reference

[Q1]: Alfred Olbrich, Netze — Protokolle — Spezifikationen, 2003

[Q2]: Hartmut König, Protocol Engineering, 2003

[Q3]: Tanenbaum & Wetherall, Computer Networks - Fifth Edition, 2011

[1]: C. Bourhfir et. al., Automatic executable test case generation for extended finite state machine protocols, 1997

[2]: Tae-hyong Kim et. al., Automatic Test Case Generation of Real Protocols: Framework and Methodology, 1998

[3]: C. Bourhfir et. al., A guided incremental test case generation procedure for conformance testing for CEFSM specified protocols, 1998

