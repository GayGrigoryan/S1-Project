
# Library Reservation System

## Table of Contents
- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [How to Run](#how-to-run)
- [Example Interaction](#example-interaction)
- [Notes](#notes)
- [Data File Format](#data-file-format)

## Overview
Welcome to the Library Reservation System GitHub repository. This C program allows users to search for a book, reserve it, and manages the library inventory and user data accordingly.

## Prerequisites
1. **C Compiler Installation:**
   Verify that you have a C compiler installed on your system. If not, install one compatible with your operating system.

2. **File Names and Paths:**
   Adjust file names and paths in the source code if necessary, based on your environment and file locations.

## How to Run
1. **Compile the Code:**
   Open your terminal or command prompt, navigate to the project directory, and compile the code using your C compiler. 
   

2. **Follow the Prompts:**
   - Enter your name and surname when prompted.
   - Enter the book title and author.
   - Decide whether to reserve the book by typing "yes" or "no" when prompted.

3. **Review Results:**
   - If you choose to reserve the book, the system will update the data files:
     - Decrease the quantity of the reserved book in the main data file.
     - Add your name, surname, reservation date, and book name to a separate user data file.
   - The system will display the reservation details, including the return date.

4. **Exit the Program:**
   To exit the program, follow the instructions and wait for the message "Exiting the CLI."

## Example Interaction

```plaintext
Enter your name and surname, please: John Doe
Enter the book: The Great Gatsby
Enter the author: F. Scott Fitzgerald
Would you like to reserve the book? (yes/no): yes
User file updated successfully. You reserved that book, you can take it today from our library. You have to bring it back on 21/12/2023
Exiting the CLI.
```

## Notes

- This system uses text files for storing book data and user reservations. Adjust file names and paths in the code if needed.
- Feel free to modify the code to suit your specific requirements or contribute to its improvement.

## Data File Format

The input data file (`data.txt`) should follow the format:

```
bookTitle,author,quantity
```

For example:
```plaintext
To Kill a Mockingbird,Harper Lee,5
1984,George Orwell,8
The Great Gatsby,F. Scott Fitzgerald,10
The Catcher in the Rye,J.D. Salinger,10
Brave New World,Aldous Huxley,10
The Lord of the Rings,J.R.R. Tolkien,10
The Hobbit,J.R.R. Tolkien,10
The Da Vinci Code,Dan Brown,10
The Chronicles of Narnia,C.S. Lewis,10
```

