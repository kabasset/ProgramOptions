# Kast::ProgramOptions

> Boost.ProgramOptions, simplified.

Kast.ProgramOptions is a single-header library for simple program options handling.

Where Boost.ProgramOptions is full-featured yet cumbersome,
Kast.ProgramOptions focuses on ease of use of fundamental features:

| Feature | Example |
| ------- | ------- |
| Positional options | `options.positional<int>("positional", "A positional option")` |
| Named options | `options.named<int>("named", "A named option")` |
| Flags | `options.flag("flag", "A flag")` |
| Short forms | `options.named<int>("short,s", "A short option")` |
| Default values | `options.named("default", "A defaulted option", 42)` |
| Parsing | `options.parse(argc, argv)` |
| Access | `options.as<int>("named")` |

See [the example program](ParseProgramOptions.cpp) for a demonstration.

