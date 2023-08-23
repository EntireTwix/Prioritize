# Priotize

just a quick test program as I mess around with imgui, although thats not to say this program lacks utility for simple task prioritization.
This program is targetted towards those who want to prioritize their academic or recreational lifes.
![full example](https://github.com/EntireTwix/PrioritizationGUI/blob/main/primary_image.png)

## Usage

### Dashboard

![image of dashboard](https://github.com/EntireTwix/PrioritizationGUI/blob/main/dashboard.png)

when a task is completed the user can check the checkbox which will instantly update the rankings.
Note: for satisfaction the percentage is color cordinated, the closer to red being of higher priority.

![image of multiple done dashboard](https://github.com/EntireTwix/PrioritizationGUI/blob/main/dashboard_partial.png)

### Values

The way it works is there are a set of values with differing weights that are to apply to each task, this is found in `Edit->Values`

![image of edit values](https://github.com/EntireTwix/PrioritizationGUI/blob/main/editing_values.png)

an overall score is assigned using these values for each task and given a priority percentage comparative to the rest of
the assignments.

### Tasks

You can edit the Tasks and their enumerations for each value on the `Edit->Tasks` page.

![image of edit tasks](https://github.com/EntireTwix/PrioritizationGUI/blob/main/editing_task.png)

### Saving&Loading

to save/load use `File->Open` and `File->Save a Copy` respectively, you will be prompted with the option to enter in a folder path, this folder path must exist beforehand

After having Opened from a location upon closing the program will Auto-Save, `File->Save a Copy` will explicitly save a copy in another location but Autosave will still be in effect on the first location

### Enums

lastly for those of you who really like customization you can also modify the enumerations float value.
For example Low, Medium, High
visually depicted as this graph, making it more steep, flatter, perhaps plateaued in the center, exeggerate the end, etc

![image of plot](https://github.com/EntireTwix/PrioritizationGUI/blob/main/editing_enums.png)

I do not recommend most users mess with the enums

## Tips

- if you notice a task is ranked higher then another task and your intuition tells you its priority is incorrect then notice what about it is overrated and adjust the value accordingly, doing so overtime will ideally fine tune your values to your preference. For example, in the example imagine my urgency value is at 50% because its quarentine and my teachers dont care about my stuff being late.
- seperate your tasks by save folders, and to have different values per grouping, to seperate your work and play for example.
- a thing to keep in mind with having a time sensitive value such as Urgency seen in the examples is that to keep the priority accurate you must upkeep its enumaration by continually updating each task when a significant time has elapsed

## Build

if you dont already have OpenGL

```
sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev
```

after that do

```
git clone --recurse-submodules https://github.com/EntireTwix/Prioritize
cd Prioritize
mkdir build
cd build
cmake ..
make
sudo ./priority
```

## Binaries

if you dont know how to build a program executables are supplied in each release.
