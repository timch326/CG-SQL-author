#!/bin/bash

set -o errexit -o nounset -o pipefail

readonly SCRIPT_DIR_RELATIVE=$(dirname "$0")

RAILROAD_PATH=${RAILROAD_PATH:-~/rr-2.0-java11}

guard_against_missing_rr() {
  if [ ! -f $RAILROAD_PATH/rr.war ]; then
    cat <<EOF >&2
ERROR: Bottlecaps's railroad diagram is missing

You can override the \$RAILROAD_PATH environment variable to use your custom path.

INSTALLATION INSTRUCTIONS:
(1) Setup Java (Macos)
$ brew install java
Don't forget to create a symlink for the system Java wrappers to find this JDK.
Follow the instructions provided after the installation. It should looke like this:
$ sudo ln -sfn /opt/homebrew/opt/openjdk/libexec/openjdk.jdk /Library/Java/JavaVirtualMachines/openjdk.jdk

(2) Install railroad diagram generator
$ wget https://www.bottlecaps.de/rr/download/rr-2.0-java11.zip
$ unzip rr-2.0-java11.zip -d ./rr-2.0-java11
$ mv ./rr-2.0-java11 ~/rr-2.0-java11
EOF
    exit 1
  fi
}

guard_against_missing_rr

cat $SCRIPT_DIR_RELATIVE/meta_licence_header.template.html
java -jar "${RAILROAD_PATH}/rr.war" <(cat -)
