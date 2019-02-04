_Note: This document is designed to render nicely with GitHub-flavored Markdown.  My apologies in advance to everyone else._

Build System Concepts
=====================

Goals of this document
----------------------

Be deleberate about choosing a build system; explain criteria, options, choices, and decisions.

Strategy to choose a build system is:
1. Enumerate good experiences to retain
2. Enumerate bad experiences to avoid
3. Be judiciously terse; the longer this is, the fewer people will read it
4. Enumerate & experiment with options
5. Make decision


Definitions
-----------

**Contributor:** Who is a contributor?  Is there some base-level knowledge we can expect?


Requirements
------------

* Minimal dependencies as practical, to facilitate easy building by others
* Cross-platform
* Any external dependencies must be freely installable (i.e., no registration, no license codes, no money; see also `CONTRIBUTING.md`)
* Any internally satisfied dependencies must be freely runable (i.e., no license codes; see also `CONTRIBUTING.md`)
* Is maintainable by contributors given a reasonable learning curve


Good experiences to retain
--------------------------

_Disclaimer: A good or bad experience in any particular build system is not an implicit vote for or against that build system; these days, how you use build systems is just as important as which one you choose._

**Single build system for everything:** (ABK) Used Waf to create a single build system for the entire project; fetching dependencies, configuring, building, testing, packaging, all from just `waf configure --platform=<...> && waf`.

**Fully internal dependencies:** (ABK) Built an ad-hoc package manager (thin wrapper on top of `git clone`) into the project's configure stage so that the project can compile on any computer for any platform (within reason).  This includes required 3rd party libraries, required 3rd party tools, you name it.  Setting up a developer machine no longer requires any special work past SSH keys, a working compiler and such.


Bad experiences to avoid
------------------------

**Boost version difference broke build:** (ABK) While onboarding a new developer, a difference in a Boost version caused an obscure build failure.  Takeaway: when practical, always freeze dependencies using some sort of package manager.  (Is this takeaway actually practical in an open-source project where the dependencies are almost by definition non-frozen?)


Available build systems
-----------------------

| Name | Namesake | Overview | Notable Advantages | Notable Disadvantages |
| ---- | -------- | -------- | ------------------ | --------------------- |
| [A-A-P](https://en.wikipedia.org/wiki/A-A-P)                  | | Python-based package manager written by the author of vim. | Augmentable via scripting (python), supports uploading and downloading files | Not designed for building software
| [Ant](https://en.wikipedia.org/wiki/Apache_Ant)               | "Another Neat Tool" | Java+XML-based build system | Widely used, straight-forward, relatively easy to read if you don't know Ant. | Build specification file is not as powerful as other options here; extensions required if you want to improve things.<br />Non-Java languages feel second-class<br />Ant extensions are developed using a different SDK than the main build specification file.<br />
| [Autoconf](https://en.wikipedia.org/wiki/Autoconf) & [Automake](https://en.wikipedia.org/wiki/Automake) | auto wrappers on top of make | Automates the creation of Makefiles for C-based projects | Widely used | Complexity, difficult to debug
| [Bazel](https://en.wikipedia.org/wiki/Bazel_(software))       | Anagram of Blaze, Google's internal build system | Java-based Build system using Skylark (flavor of Python)-based configuration files for describing the build | Parallel by default, supported by Google, (computer) language-agnostic. Repeatable, scalable builds<br />Xcode support: bypasses Xcode to an extent; builds various Apple components manually
| [Buck](https://en.wikipedia.org/wiki/Buck_(software))         | Facebook's internal build system | | Native integration with Xcode and IntelliJ | Native integration in only Xcode and IntelliJ
| [BuildAMation](https://en.wikipedia.org/wiki/BuildAMation)    | | A modular build system using C#-like syntax
| [Buildr](https://en.wikipedia.org/wiki/Apache_Buildr)         | | A Ruby-based competitor to Ant. | | | Intended primarily for Java projects
| [CMake](https://en.wikipedia.org/wiki/CMake)                  | | A cross-platform, modular wrapper around Make
| [Gradle](https://en.wikipedia.org/wiki/Gradle)                | | | | Intended primarily for Java, Groovy, and Scala
| [GYP](https://en.wikipedia.org/wiki/GYP_(software))           | Written by Google; used by Chrome and NodeJS | Native IDE project file generator. Used by the V8 Javascript engine, Google's web browser (Chrome), Dart (programming language), Node.js, WebRTC Telegram, and Electron.
| [make](https://en.wikipedia.org/wiki/Make_(software))         |
| [Meson](https://en.wikipedia.org/wiki/Meson_build_system)     | | | Good support of clerical tasks (Xcode integration, VS integration, Apple installer packages, etc) | Cross-platform insulation layer for compiler parameters seems to be a bit naive at first glance
| [NixOS](https://github.com/NixOS/nixpkgs)                     | | | Idempotent package management and build system with atomic releases/rollbacks and human readable configs (with integrated bash scripting) that are easily source controlled. Comes with integrated release distribution, and can even be used as our server OS, which would give us the same benefits with user management and system state (including replacing cron). | Linux only
| [Premake]( https://en.wikipedia.org/wiki/Premake)             | | An Xcode/VS/etc project file generator | | Only a project file generator
| [Rake](https://en.wikipedia.org/wiki/Rake_(software))         | | Ruby-based competitor of SCons | | Lacking strong IDE integration
| [SCons](https://en.wikipedia.org/wiki/SCons)                  | | Predecessor of Waf | | KDE used SCons for a while but decided to switch to CMake for scalability reasons.
| [Tweaker](https://en.wikipedia.org/wiki/Tweaker_(build_tool)) | | Competitor of SCons
| [Waf](https://en.wikipedia.org/wiki/Waf)                      | | Successor of SCons (written by same author); Waf is a python-based general-purpose parallel task execution engine; includes "tools" for common tasks, such as C/C++ compiling. Relatively easy to write generic tasks. | Highly modularized; easily extensible | Highly modularized; easily extensible (bad for complexity/maintenance; easy to shoot yourself in the foot)
