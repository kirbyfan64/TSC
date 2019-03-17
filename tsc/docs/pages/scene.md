Creating scenes
===============

In order to narrate the game story, TSC supports two kinds of scene
systems. The primary one is a visual novel system and described in
this document. The other one is based on scripting and allows you
construct small story scenes directly in the level. Since the visual
detail of level graphics is limited, any larger story narration should
use real scenes as described in this document. Documentation for the
scripting scene system can be found in the scripting documentation for
class `CinematicScene`.

Scene authors are asked to read I. and II. below. Scene implementors
should also read III.

I. Basics
---------

TSC's scene system is implemented as a special game mode (`MODE_SCENE`
in the C++ code). When a scene is activated, any other gameplay is put
to halt, a transition occurs (darkening the screen and reopening
view), and a large full-screen image is shown to the user. Then, story
is narrated using a story text box shown in the lower third of the
screen. The user can advance the story text by pressing either of
Return, the Action, or the Shoot key. Depending on the
configuration of the scene, new text appears, the full-screen image
changes, or something else happens on advancement.

A scene consists of an action sequence that is to be played when the
scene is entered. Each action can do something in the scene, and once
all actions have finished execution, the scene exits.

When a scene exits, what happens depends on the scene in
question. Scenes that were called up from a running level can return
to that level, resuming gameplay in the level exactly where it was
left off. The scene can also be configured to warp the player to
another level entry. It is also possible to load a level (again,
optionally with setting the player to a specific level entry), or to
show the credits screen. By default, scenes assume to be called from
an active level and that gameplay should be continued where it was
left off. If that is not the case for your scene, it is required to
tell the scene what should happen afterwards, otherwise TSC is going
to crash when trying to resume the non-existing active level
(`pActive_Level` will be a NULL pointer).

II. Drafting a Scene
---------------------

Before a scene can be implemented in code, it needs to be drafted. As
outlined above, a scene consists of actions to be executed. Drafting a
scene thus means to draft the action sequence the scene shall execute
when it is entered. Possible actions include:

* Change of background image
* Change of background music
* Wait a specified amount of time
* Narrate story text

The full list of available actions can be inferred from
`scene/scene_actions.hpp`, though the mentioned ones are the most
important ones. If you're not a programmer, you can just ask some of
the TSC coders to look up for you if there's an action available for
what you would like to do.

A full scene draft contains these elements:

* All images to be shown during the course of the scene, in SVG
  format. It is assumed that scene images are not re-usable across
  multiple scenes, so you probably can't do without drawing.
* All music files to be played during the course of the scene, in OGG
  Vorbis format. It is likely that you will just use music already
  included in TSC, thus naming existing music files is sufficient.
* The action sequence.
* When the scene should be entered.
* What should happen when it exits.

The action sequence draft should be a numbered list of actions. It
should look like the following example:

> 1. Change background music to `story/theme_1.ogg`.
> 2. Change background image to `scene_images/papers.png`.
> 3. Wait 5 seconds.
> 4. Narrate story:
>   > The first ray of light which illumines the gloom, and converts
>   > into a dazzling brilliancy that obscurity in which the earlier
>   > history of the public career of the immortal Pickwick would
>   > appear to be involved, is derived from the perusal of the
>   > following entry in the Transactions of the Pickwick Club...✝
> 5. Change background to to `scene_images/transcript.png`.
> 6. Narrate story:
>   > May 12, 1817. Joseph Smiggers, Esq., P.V.P.M.P.C. presiding.
>   > The following resolutions unanimously agreed to.✝

✝ The example texts constitute the opening of _The posthumous papers
of the Pickwick Club_, Chapman & Hall 1837, by Charles Dickens.

Once a full story draft has been submitted and accepted, it can be
implemented into a real scene (see below). As an open-source project,
TSC can't demand full story drafts from every occasional contributor,
but try to get as close as reasonably possible. For any scene images
you can't draw yourself, you can specify the placeholder image
`scene_images/placeholder.png` to be shown. It is ugly enough to press
everyone to replace it soon.

III. Implementing a Scene
-------------------------

Scenes are implemented as simple C++ classes derived from the
`TSC::cScene` class. Scenes are placed as a pair of cpp/hpp source
files in the `scenes/` directory and given a number at the beginning
of their file names, as to faciliate imagining the order in which the
scenes are to appear in the game.

The scene is implemented by filling the action sequence in its
constructor. A typical scene constructor looks like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
cMyScene::cMyScene(void)
  : cScene()
{
  // Create a shortcut so you don't have to write out
  // TSC::SceneActions all the time.
  namespace TSA = TSC::SceneActions;

  // Append to action sequence
  m_action_sequence.push_back(new TSA::ImageChange(this, "scene_images/foo.png"));
  // ...

  // Set what happens after the scene finished
  m_action_sequence.push_back(new TSA::NextUp(this, GA_ENTER_LEVEL, "lvl_78"));
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

That is, the action sequence as provided with the scene draft needs to
be translated into the creation of instances of subclasses of
`TSC::SceneActions::Action`. It is recommended to read
`scene/scene_actions.hpp` to get an overview of the possible actions.

There's usually no need to implement any other methods in a scene
subclass than the constructor, as scenes are linear currently.

The first thing any scene should do is to issue the
`TSC::SceneActions::ImageChange` and `TSC::SceneActions::MusicChange`
actions, otherwise only a black screen is shown and whatever was the
previous background music continues to play.

The last thing any scene should do is to issue the
`TSC::SceneActions::NextUp` action, unless the scene is okay with
resuming the last active level from the point where it was
halted. This action defines what happens after the scene has finished;
options include resuming the level (default), changing the level, or
advancing on the overworld.

Story text narration deserves an additional comment. The
`TSC::SceneActions::Narration` class' constructor takes an
`std::initializer_list` of strings, each of which will become exactly
one story box content. That is, each of the strings has to fit into
the story box. Don't use overly long strings. It is not required to
manually break lines with a `\n` unless you're really uncomfortable
with how CEGUI breaks the lines.

Story text needs to be translatable. Use the `_()` macro from
`i18n.hpp` to mark any story text as translatable. If you insert a
manual `\n`for an optically improved line break, you also need to add
a `// TRANS:` comment to notify translators that this line break only
serves optical purpose and must not be copied into the translated text
if the line break looks good there already:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ c++
m_action_sequence.push_back(new TSA::Narration, this {
  _("This line is broken okay. This line is broken okay. This line is broken okay. This line is broken okay."),
  // TRANS: Line break only for optical purposes. Remove if not
  // TRANS: required in your translation.
  _("This line has a problem and it is required to manually\nbreak the text to have it look properly.")
  });
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`xgettext`, the programme used to search the source code for
translatable strings, extracts `TRANS:` comments and places them above
the translatable string. Translators can thus handle the situation
properly.
