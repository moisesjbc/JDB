# Adding dangers to the game

## Dangers as states machines guided by events

### Events

#### User actions

This event triggers when the player uses a tool on a danger. For example: when the player uses the **hand** tool over a **knife** danger.

#### Timeout

Triggered when a certain time passes.

#### End of animation

Triggered when the current animation state of the danger ends.

## Danger parameters

- **DamageFactor:** used when a danger reachs the grinder. When this happens, Jacob recives a damage equal to `<danger life> * <damage factor>`
- **Base line:** explained in next section **Sandwiches as dangers containers**.

**TODO:** Add the rest of parameters.

## Sandwiches as dangers containers

Technically speaking, a sandwich is nothing more than a container for dangers. **When the game creates a new sandwich, this is populated with dangers using a horizontal base lines system**.

- Every sandwich has a horizontal base line indicating where the dangers can be placed on its surface.
- Every danger has its own horizontal base line indicating which part of it must be in touch with the sandwich.

### Populating process

When a new sandwich must be populated with dangers, a random set of n or less dangers are selected, so the sum of the width of their base lines is equal or less than that of the sandwich. If there is free space as a result of this process, the dangers are randomly offset to one side or another.

The following images shows the use of base lines for populating a sandwich with dangers:

![](sandwich_population.png)
