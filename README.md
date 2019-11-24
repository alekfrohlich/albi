# Albi

Albi is a reaction extractor. It's goal is to parse a subset of Gro that is semantically meaningful to simulators and synthesis tools cabable of handling the SBML standard.

## Installing

### Installing dependencies

Albi's dependencies are limited to ```telurium``` and ```roadrunner```, to install them run:

```$ pip3 install -r requirements.txt```

### Building

To build the framework change directory to ```assignment```. Being there, run ```make```, `translator` should apear under ```assignment/src/```.

## Running

To build an SBML document from gro source code, run:

```$ python3 albi path/to/gro/file```

The resulting file is output.xml.

## Documentation

A BNF grammar for Albi is available under ```docs/albi.bnf```

The git-style used for creating branches and issues is located under ```docs/git-style.md```
