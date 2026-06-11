# 🏏 Cricket Scorecard Management System

## Overview

The Cricket Scorecard Management System is a console-based application developed in C++ that simulates a complete cricket match with real-time scoring and detailed statistical analysis.

The project allows users to create teams, conduct a toss, select pitch conditions, record ball-by-ball match events, and generate comprehensive batting and bowling scorecards. The system is designed using Object-Oriented Programming principles and demonstrates practical implementation of core software engineering concepts.

---

## Features

### Match Simulation

* Team creation and player management
* Toss simulation
* Pitch selection (Batting-Friendly / Bowling-Friendly)
* Multi-over match support
* Real-time score updates

### Batting Statistics

* Runs scored
* Balls faced
* Strike rate calculation
* Boundaries (4s and 6s)
* Fall of wickets tracking
* Partnership monitoring

### Bowling Statistics

* Overs bowled
* Runs conceded
* Wickets taken
* Economy rate
* Dot balls
* Maiden over detection

### Match Events

* Wickets with multiple dismissal types
* Wide balls
* No balls
* Byes
* Leg byes
* Run-outs
* Catch handling with ratings
* Player injury management
* Retired hurt scenarios

### Advanced Features

* Fantasy points calculation
* Best catch recognition
* Hat-trick detection
* Match summary generation
* Player performance ratings

---

## OOP Concepts Implemented

This project demonstrates the practical use of the following Object-Oriented Programming concepts:

### Encapsulation

Player, Team, and Match data are encapsulated within their respective classes.

### Inheritance

The `Player` class inherits from the `Cricketer` base class.

### Polymorphism

Virtual functions are used to demonstrate runtime polymorphism.

### Abstraction

Complex match operations are abstracted into reusable class methods.

### Operator Overloading

Custom operator overloading is implemented for player comparison and display functionality.

### Templates

Template-based utility functions are used to improve code reusability.

---

## Project Structure

```text
cricket-scorecard-management-system/
│
├── src/
│   ├── main.cpp
│   ├── Cricket.cpp
│   └── cricket.h
│
├── output/
│   └── Output.pdf
│
└── README.md
```

---

## Technologies Used

* C++
* Standard Template Library (STL)
* Object-Oriented Programming
* File Handling
* Console-Based User Interface

---

## How to Compile and Run

### Compile

```bash
g++ main.cpp Cricket.cpp -o cricket
```

### Run

```bash
./cricket
```

---

## Sample Output

The repository includes an output PDF demonstrating the complete execution flow of the application, including:

* Team creation
* Toss and pitch selection
* Ball-by-ball scoring
* Batting scorecards
* Bowling scorecards
* Match result generation

---

## Learning Outcomes

Through this project, the following concepts were explored and implemented:

* Object-Oriented Design
* Class Relationships
* Match State Management
* Statistical Computation
* Real-Time Event Processing
* Modular Software Development

---

## Future Enhancements

* Match data storage using files or databases
* Tournament management system
* Graphical User Interface (GUI)
* Player performance analytics dashboard
* Historical match records
* Multiplayer score management

---

## Author

**Sasindra Sri Sai Nandam**

B.Tech – Computer Science and Engineering Major in Artificial Intelligence

Indian Institute of Information Technology Design and Manufacturing (IIITDM) Kancheepuram

---

## License

This project was developed as part of an academic Object-Oriented Programming course and is intended for learning and demonstration purposes.
