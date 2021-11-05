#!/bin/bash

# for old jre
# java -jar fsm-editor.jar

# for new jre
java -cp "jaxb-api-2.3.0.jar:jaxb-core-2.3.0.jar:jaxb-impl-2.3.0.jar:fsm-editor.jar:activation-1.0.2.jar"  com.mxgraph.examples.swing.SCXMLGraphEditor
