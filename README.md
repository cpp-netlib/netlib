# netlib

Join us on Slack! http://slack.cpp-netlib.org/

Subscribe to the mailing list: https://groups.google.com/forum/#!forum/cpp-netlib

This is a re-do of the work in cpp-netlib/cpp-netlib but updated to the latest
C++ standard. This repository is a from-scratch re-imagining of the solution to
modern C++ network programming.

IMPORTANT: This is not an officially supported Google product.

## Goals

* Provide modern composable components for network programming. We'll provide
  the middle-level abstractions (connections, messages, encoders/decoders) which
  will work with lower-level abstractions provided by platform-specific
  interfaces (POSIX, etc.).

* Bundle higher level protocol implementations that build on top of the
  composable components. This will include a re-imagining for an HTTP client and
  server implementation, supporting HTTP 1.0/1.1/2 and even work that's at the
  bleeding edge of HTTP/3 (QUIC).

* Use the latest C++ standard. There will be no backwards compatibility
  concerns. This is a departure from the earlier attempt at keeping a stable
  interface and implementation.

* Develop and Release at HEAD. There will be no development/release branches.
  All work will be done at HEAD (the `master` branch) and shall be 

## Status

As of January 2019, this repository is empty, and work will commence as soon as
we establish some of the community guidelines. There are some important changes
from the cpp-netlib/cpp-netlib repository which are worth highlighting:

* License change. We will not be re-using anything from the
  cpp-netlib/cpp-netlib repository because of licensing differences. The
  `netlib` repository will be licensed under the Apache 2.0 License with a
  contributor license agreement for contributors. We've decided to do this to be
  more accessible and in-line with the prevailing practice for new open source
  projects, with better options for sustainable development and hygiene.
* Repository layout. We will be following a simpler repository/code layout, with
  a flat structure and using subdirectories for logical grouping. This gives us
  flexibility in the way we add new implementations and simplify the development
  process.
* Modern practices. We will use continuous integration, test-driven-development,
  with a focus on readable, well-tested code. This includes supporting modern
  build systems and packaging/deployment.
* Community driven process. We will involve the community in decisions and in
  the development process in a more engaged manner. We will enforce that changes
  are reviewed, that designs are documented, and that plans are tracked. Most of
  all we want to be accountable in the work we're doing and delivering for the
  community of users and developers.
* Regular releases. With automation in place, we will follow a regular release
  cadence that is automated, with minimal human intervention.

## Community

This time around we're going to engage the community more effectively. Instead
of a handful of maintainers making changes and decisions by fiat, we will
institute a review process which encourages everyone in the community to voice
their thoughts and concerns. We will start with the current maintainers and grow
the pool of committers as we grow the community.

We are committed to being an inclusive and encouraging environment where all
contributors who agree to be part of the process are welcome. We have a code of
conduct which we are committed to enforcing (see `CODE_OF_CONDUCT.md` for
specifics).
