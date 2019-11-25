Known Bugs
==========

This file lists known long-standing problems with TSC. The TSC team
welcomes any contribution to get these problems resolved. Where
applicable, ticket numbers are provided for the problem that refer to
tickets on the bug tracker.

## Invisible lava and crates (#565)

It is possible to create invisible lava. On creation of a new level,
the level editor automatically inserts a single ground block. If this
block is removed, no objects but Alex himself remain in the level. If
now lava objects are added and the level is saved and reloaded, the
lava is invisible and remains in that state, even if any other objects
are added later on. The invisible lava still kills Alex.

In the same fashion, it is possible to create invisible crates.

Workaround: do not insert lava or crates as the first massive object
in a level. The problem does not occur if any massive object has been
inserted before the lava or crate object.

Do not make use of this bug in any level. In particular, do not abuse
the invisible lava for death traps. This is something the player does
not expect. Any levels submitted for inclusion into the official TSC
levelset that use this bug will be rejected.
