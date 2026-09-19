#!/bin/sh
set -eu

cd "$(dirname "$0")/../html"

xsltproc -o modules.xhtml anlmodules.xsl modules.xml 
