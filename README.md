# Ticker Lines Application

This application demonstrates the use of threads and synchronization using a mutex object. Multiple instances of the application can be started, and each thread outputs a "ticker line" string. Synchronization ensures that only one thread at a time can perform a two-fold string output operation.

## Key Features

- Multi-threaded application
- Mutex synchronization for thread-safe output
- Supports multiple instances of the application

## Example

The application outputs "ticker lines" using synchronized threads. When multiple threads are active, they are coordinated using a mutex, ensuring each thread gets exclusive access to the console for printing.

### Ticker Line Operation:
- Each thread outputs a line of text in two parts.
- Mutex guarantees atomic execution, preventing race conditions.

## GIF Preview

![sync_threads1](https://github.com/user-attachments/assets/ticker_lines_animation.gif)
