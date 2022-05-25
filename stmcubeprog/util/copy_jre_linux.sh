#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cp -r jre $DIR/../bin/jre
chmod +x $DIR/../bin/jre/bin/java