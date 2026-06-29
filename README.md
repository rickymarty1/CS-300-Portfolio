# CS 300 Portfolio

This repository has my portfolio work for CS 300 Data Structures and Algorithms. It includes my Project One runtime and memory analysis and my Project Two C++ advising program.

## Reflection

The main problem in these projects was figuring out how to organize course information for an advising program. The program had to read course data from a file, store the course number, title, and prerequisites, then print the courses in alphanumeric order. It also had to let the user search for one course and see what prerequisites it had.

I started by looking at different data structures. In Project One, I compared vectors, hash tables, and binary search trees. Each one stores and finds data in a different way. That matters because the data structure can change how fast the program loads, searches, sorts, and prints information. For this project, the binary search tree made the most sense because the program needed to print the course list in order.

One roadblock was getting the input file to work right. The file had course numbers, course titles, and different numbers of prerequisites. Some courses had none. Some had more than one. So the program had to read each line and handle those cases without breaking. Testing helped a lot here. I had to run the program, see what failed, fix it, and test it again.

This project changed how I think about writing programs. I can see why it helps to plan the structure before jumping straight into code. I had to think about where the data would go, how the user would move through the menu, and what each function needed to do. Once that was clear, the final program was easier to build.

This work also helped me write cleaner code. I tried to keep each function focused on one job, like loading the file, printing the course list, searching for a course, or showing the menu. I also used comments to explain the main parts of the program without making the code feel crowded. That makes the program easier to read now, and it would make it easier to change later if more courses or features were added.
