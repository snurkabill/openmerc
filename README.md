# Mercore

[![Join the chat at https://gitter.im/mrzahrada/mercore](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/mrzahrada/mercore?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/mrzahrada/mercore.svg?branch=master)](https://travis-ci.org/mrzahrada/mercore)

## About

Mercore is brought to you by Openmerc community. Openmerc helps you to get done with your activities in trading. You are about to explore all-in-one trading solution with unlimited possibilities. Mercore is the one and only instrument that serves the Openmerc community. 

## Issues and Fixes

At the moment, we are not experiencing any bugs and issues. Help us to dig them out. 

## Installation

Regular course of installation begins with
`git clone https://github.com/mrzahrada/mercore`

Then use CMakeLists.txt as following 
`cmake .`

Finish compiling with
`make`


## Requirements

To run mercore properly, you will need other essential structures:

* Boost Libraries v. 1.57.0
* [ØMQ Libraries](http://github.com/zeromq/libzmq) v. 4.0.5
* [ØMQ C++ Driver](http://github.com/zeromq/cppzmq) v. 4.1 
* [docopt](http://github.com/docopt/docopt) v. 0.6.1 
* g++ v. 4.8 *or* clang++ v. 3.7
* [MongoDB](http://github.com/mongodb/mongo) v. 2.4.12 
* [Mongo C++ Driver](http://github.com/mongodb/mongo-cxx-driver) v. 1.0.0 

You can prevent hours of frustration if you install all these structures in recommended versions. The first version of mercore is tested on Debian and Ubuntu.

## Components

Mercore comprises three basic components:
- Shell represents user interface.
- Core keeps the data and share them among modules, operates a database and transforms inputs and outputs to the shell so you can control it. Literally, the core component.
- Modules are most frequently dynamic-link libraries employing three basic methods (initialize, update, deinitialize). The modules are widely modifiable to serve the very specific purposes. They can be engaged as a broker connection, strategy manager, backtest module and so on.

## Running

To print help and see the guidelines of common usage, type command 

`oms.py --help`
 
or

`oms.py --h`

## Examples

We are going to add examples of mercore utilisation later.

## Documentation


## License

Mercore runs under the MIT License with respect to licences of included code. For more information on MIT License, see [opensource.org](http://opensource.org/licenses/MIT) or [LICENSE](https://github.com/mrzahrada/mercore/blob/master/LICENSE) file.
Namely, following parts differ from the source code license:

* Boost Libraries - Boost Software License; Version 1.0, August 17th, 2003
* ØMQ driver - MIT License 
* docopt - MIT License
* MongoDB C++ Driver - Apache License, version 2.0

## Manifesto

## Contributing

The instructions for cooperation will be detailed soon. 



with &hearts; by openmerc team.
