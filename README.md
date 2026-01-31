"# SkyNet-Air-Traffic-Control-ATC-Simulator" 
"# SkyNet-Air-Traffic-Control-ATC-Simulator" 
Project Description — SkyNet Air Traffic Control System

SkyNet is a console-based Air Traffic Control (ATC) simulation system developed in C++, designed to demonstrate the practical implementation of core data structures in a real-world scenario. The system manages aircraft movement, landing priorities, airspace routing, and flight records while ensuring safety and efficiency.

The project integrates multiple data structures—such as Graphs, Min Heaps, Hash Tables, AVL Trees, and Radar-based tracking—to simulate how an air traffic control system monitors aircraft, handles emergencies, schedules landings, and prevents collisions.

SkyNet provides a menu-driven interface that allows users to add flights, track aircraft in real time, declare emergencies, find safe routes, and maintain structured flight logs.

✈️ System Capabilities

Aircraft registration and tracking

Priority-based landing management

Emergency handling and rerouting

Safe path finding in airspace

Collision detection using radar

Persistent storage (save/load system state)

Fast search and logging of flight records

🧠 Data Structures Used

Graph
Used to model the airspace and compute safe routes between locations.

Min Heap
Manages the landing queue based on aircraft priority and urgency.

Hash Table
Provides fast lookup for registered aircraft using flight IDs.

AVL Tree
Stores and maintains balanced flight logs for efficient insertion and retrieval.

Radar System
Tracks aircraft movement and detects possible collisions in real time.

🛠️ Technologies & Concepts

Language: C++

Programming Paradigm: Object-Oriented Programming (OOP)

Concepts Applied:

Classes & Objects

Encapsulation

Dynamic memory management

Custom implementation of data structures

File handling (state persistence)

Menu-driven program design

🎯 Purpose of the Project

The purpose of SkyNet is to:

Demonstrate real-world application of data structures

Simulate a complex control system using efficient algorithms

Strengthen understanding of OOP and system design

Practice implementing DS without relying on STL containers
