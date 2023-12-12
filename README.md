# BirdExterminator

Bird Exterminator is a game created in Unreal Engine 5.3 for a job application interview. 

# Overview

This game is about a bird extermination in a city by shooting predator birds that hunt their prey and eliminate them. Aim is to exterminate all green birds (prey) by shooting with the red ones (predators) that will seek and destroy its prey. But beware, if the red bird will exhaust all of its energy by boosting, it will tranform into a green bird and you will have to exterminate them too!

# World
World is represented by a flat plane and tall cube-like buildings. It is created from a config file located in project config directory, called WorldConfig.txt (example below)
>5,6<br />
>900,800,1100,1100,900<br />
>1100,900,1000,1200,1100<br />
>700,800,700,1300,800<br />
>600,500,1500,1400,500<br />
>300,400,700,1000,1300<br />
>200,800,600,500,1000<br />

First row - buildings quantity on X axis, buildings quantity on Y axis

Next rows - every building in matrix height

# Character
Character can shoot with predator birds in order to make them hunt the prey birds.

Controls (Keyboard + Mouse / Controller):

- Moving: WSAD / Right analog stick
- Aim: Mouse / Left analog stick
- Shoot: Left mouse button / Right Trigger
- Sprint: Left Shift / Left Trigger
- Jump: Space / A

# Prey bird
Prey bird is represented as a green sphere with a cone in front (its beak). Behavior is as follows:
- Prey spawn in flocks, bird follows its place in flock and the flock is travelling through the map randomly
- Prey avoid buildings and other birds by trying to turn from it
- Prey are flying away from predators if any of them is in danger distance
- Prey can be without a flock, when was transformed from predator bird that exhausted its energy. In that case it just flies forward, aviod obstacles/predators and keeps in map bounds

# Predator bird
Predator bird is represented as a red sphere with a cone in front (its beak). They are being spawned when a player shoots it by pressing LMB / Right Trigger. Behavior is as follows:
- When spawned, Predator is flying forward with double boost mindlessly, imitating a bullet, for a short moment, in order to give a player more control of when it will start targeting
- After launching time, predator targets nearest Prey and follows it, when its close enough it activates boost for speed and turn rate, using energy (2 levels of boost and energy consume based on distance)
- When it hits a Prey, Prey is destroyed and Predator recovers some energy
- When while using a boost energy depletes, Predator transforms into Prey and also needs to be exterminated in order to win the game

# Game End
Game ends when all Prey birds are destroyed (win), or there are no Predator Birds alive and available to spawn while any Prey bird is alive (lose).
