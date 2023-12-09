# Multi-Class Producer-Consumer Synchronization

## Overview

This project addresses the multi-class producer-consumer synchronization problem in concurrent computing. The scenario involves four producer processes (P1, P2, P3, P4) and two consumer processes (C1, C2) that share a common buffer. The goal is to synchronize access to the shared buffer while adhering to specific constraints.

### Constraints

- P1 and P2 belong to the same class (CL1).
- P3 and P4 belong to another class (CL2).
- Consumers (C1 and C2) can only consume messages produced by processes in their respective classes.

## Implementation

The implementation employs a shared memory structure with a buffer to store messages and an integer to represent the class of the message producer. Semaphores are used for synchronization, preventing race conditions and maintaining the integrity of the shared buffer.

## Usage

1. Compile the program.
2. Run the executable.

## Contributors

- Mehdid Samy Abderraouf
- Belhaddad Chawki Ahmed Ramzy

## Supervisor

- F. Sabba

Feel free to explore the code and contribute to the project!
