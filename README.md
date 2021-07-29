# Mafia Mod for Real Virtuality 4

This is the repository for the *Mafia* mod for Bohemia
Interactive's [Real Virtuality 4](https://community.bistudio.com/wiki/Real_Virtuality)
engine (Arma III). It allows developers to develop modules in Javascript with full access to Arma's scripting commands
(SQF). Mafia mods can be loaded, unloaded and reloaded at runtime without having to restart the game/server.

Mafia started originally as a fork of [Intercept](https://github.com/intercept/intercept), which allows developers to
write mods in C++.

## Development Status

Mafia is currently unfinished and not yet usable. Development is done in my own time in between projects.

## Details

Mafia is mostly written in C++ with a few additional helper scrips written in Python.

A list of all available SQF functions is crawled and parsed from BI's wiki, aggregated into a single
[commands.json file](https://github.com/Ilareguy/mafia-core/blob/master/tools/commands.json), and used to generate C++
definitions and implementation details. Since each of these SQF function is a native function in Arma's binary, a
pointer can be searched for and acquired at runtime. These functions are exposed to loaded Javascript modules,
which can directly call into them.

Mafia uses [Duktape](https://duktape.org/) to interpret Javascript code.

## Other Languages

I plan on eventually writing a module interface for various other languages if there is a demand for it.
