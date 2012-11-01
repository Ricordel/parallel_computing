#!/bin/bash

file=Pi.scala

if [ ! $SCALA_HOME ]
then
    echo ERROR: set a SCALA_HOME environment variable
    exit
fi

if [ ! -f scala-library.jar ]
then
    cp $SCALA_HOME/lib/scala-library.jar .
fi

scalac -sourcepath src -d bin src/Pi.scala

cd bin
jar -cfm ../pi.jar ../MANIFEST.MF *
cd ..
