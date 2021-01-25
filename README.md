# PrioritizationGUI (2/3 complete)

![full example](https://github.com/EntireTwix/PrioritizationGUI/blob/main/primary_image.png)

just a quick test program as I mess around with imgui, although thats not to say this program lacks utility for simple task prioritization.

This program is targetted towards those who want to prioritize there academic or recreational lifes.
The way it works is there are a set of values with differing weights that are to apply to each task, this is found in `Edit->Values`

![image of edit values]

an overall score is assigned using these values for each task and given a priority percentage comparative to the rest of
the assignments.

![image of dashboard]

You can edit the Tasks and there enumerations for each value on the `Edit->Tasks` page.

![image of edit tasks]

when a task is completed the user can check the checkbox which will instantly update the rankings.
Note: for satisfaction the percentage is color cordinated, the closer to red being of higher priority'

![image of multiple done dashboard]

lastly for those of you who really like customization you can also modify the enumerations float value,
visually depicted as this graph, making it more steep, flatter, perhaps plateaued in the center, exeggerate the end, etc

![image of plot]

## Tips

- if you notice a task is ranked higher then another task and your intuition tells you its priority is incorrect then notice what about it is overrated and adjust the value accordingly, doing so overtime will ideally fine tune your values to your preference. For example, in the example images my urgency value is at 50% because its quarentine and my teachers dont care about my stuff being late.
- seperate your tasks by save folders, and to have different values per grouping, to seperate your work and play for example.
- a thing to keep in mind with having Urgency as a value or anything time sensitive is that you have to continually update tasks as days elapse atleast in the time relevant category.

## Build
```
git clone --recurse-submodules https://github.com/EntireTwix/PrioritizationGUI
mkdir build
cd build 
sudo cmake ..
sudo make
sudo ./priority
```

## Usage

if you dont know how to build a program executables are supplied in each release.
