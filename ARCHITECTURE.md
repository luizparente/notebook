# Architecture Documentation

## Overview

This document describes the architecture and design decisions behind the Notebook application. The application is built following professional software engineering principles, with a focus on maintainability, extensibility, and clean code.

## Design Philosophy

### SOLID Principles

The entire codebase is structured around the SOLID principles:

#### 1. Single Responsibility Principle (SRP)
Each module has exactly one reason to change:

- **Document** (`core/document.c`) - Manages document state (content, file path, modification status)
- **File Operations** (`io/file_operations.c`) - Handles all file system interactions
- **Clipboard Operations** (`clipboard/clipboard_operations.c`) - Manages clipboard functionality
- **Theme Manager** (`theme/theme_manager.c`) - Controls application theming
- **Application** (`core/application.c`) - Coordinates between subsystems
- **Main Window** (`ui/main_window.c`) - Handles UI presentation and user interaction

#### 2. Open/Closed Principle (OCP)
The system is open for extension but closed for modification:

- **Theme System**: New themes can be added by extending the theme data structures without modifying existing theme logic
- **Callback System**: Event handling uses callbacks, allowing new behaviors without modifying core logic
- **File Operations**: Error handling is extensible through error codes and messages

#### 3. Liskov Substitution Principle (LSP)
Components are designed with interfaces (via function pointers and abstract types):

- Opaque pointer types (`Document*`, `Application*`, etc.) hide implementation details
- Functions operate on interfaces rather than concrete implementations
- Substitutability is maintained through consistent contracts

#### 4. Interface Segregation Principle (ISP)
Interfaces are focused and minimal:

- Document interface only exposes document-specific operations
- File operations interface is separate from clipboard operations
- Theme management is independent from other concerns
- No client is forced to depend on methods it doesn't use

#### 5. Dependency Inversion Principle (DIP)
High-level modules depend on abstractions:

- `Application` depends on abstract interfaces (`Document*`, `ThemeManager*`, `ClipboardOperations*`)
- `MainWindow` depends on `Application` abstraction, not concrete implementations
- Low-level modules implement the abstractions defined by high-level needs

## Architectural Layers

### Layer 1: Infrastructure Layer
**Purpose**: Provides low-level services and platform abstractions

**Components**:
- `file_operations.c` - File I/O (read, write, file checks)
- `clipboard_operations.c` - System clipboard integration
- `theme_manager.c` - Theme data and CSS management

**Characteristics**:
- No dependencies on other layers
- Can be tested independently
- Platform-specific code isolated here

### Layer 2: Domain Layer
**Purpose**: Implements core business logic and domain models

**Components**:
- `document.c` - Document entity and business rules

**Characteristics**:
- Platform-independent
- Pure business logic
- No UI dependencies
- Testable without infrastructure

### Layer 3: Application Layer
**Purpose**: Coordinates use cases and workflow

**Components**:
- `application.c` - Application controller/coordinator

**Characteristics**:
- Orchestrates infrastructure and domain layers
- Implements application workflows (open, save, new document)
- Manages subsystem lifecycle
- Provides unified interface to UI layer

**Dependencies**:
```
Application
├── Document (domain)
├── FileOperations (infrastructure)
├── ClipboardOperations (infrastructure)
└── ThemeManager (infrastructure)
```

### Layer 4: Presentation Layer
**Purpose**: Handles user interface and interaction

**Components**:
- `main_window.c` - GTK-based UI implementation

**Characteristics**:
- Depends only on Application abstraction
- Implements Observer pattern for updates
- Translates user actions to application commands
- Updates UI based on application state changes

## Data Flow

### Opening a File
```
User Action (File -> Open)
    ↓
MainWindow: on_open_activated()
    ↓
MainWindow: Choose file dialog
    ↓
Application: application_open_document(path)
    ↓
FileOperations: file_operations_read(path, doc)
    ↓
Document: document_set_content() + document_set_file_path()
    ↓
Application: Callback: on_document_loaded()
    ↓
MainWindow: Update text view and title
```

### Saving a File
```
User Action (File -> Save)
    ↓
MainWindow: on_save_activated()
    ↓
MainWindow: Get text from buffer
    ↓
Application: application_save_document()
    ↓
FileOperations: file_operations_write(path, doc)
    ↓
Document: document_mark_saved()
    ↓
Application: Callback: on_document_saved()
    ↓
MainWindow: Update title (remove asterisk)
```

### Theme Toggle
```
User Action (View -> Toggle Theme)
    ↓
MainWindow: on_toggle_theme_activated()
    ↓
ThemeManager: theme_manager_toggle()
    ↓
ThemeManager: Callback: on_theme_changed()
    ↓
MainWindow: main_window_apply_theme()
    ↓
GTK CSS Provider: Apply new theme CSS
```

## Design Patterns

### 1. Model-View-Controller (MVC)
- **Model**: `Document` - Contains application data and state
- **View**: `MainWindow` - Presents data and captures user input
- **Controller**: `Application` - Mediates between model and view

### 2. Observer Pattern
Used for event notification:

```c
// Theme change notification
typedef void (*ThemeChangeCallback)(ThemeType theme, void* user_data);

// Document event notification
typedef struct {
    void (*on_document_modified)(void* user_data);
    void (*on_document_saved)(void* user_data);
    void (*on_new_document)(void* user_data);
    void (*on_document_loaded)(void* user_data);
    void (*on_error)(const char* message, void* user_data);
} ApplicationCallbacks;
```

### 3. Facade Pattern
`Application` acts as a facade:
- Simplifies complex subsystem interactions
- Provides unified interface for UI layer
- Hides implementation details

### 4. Strategy Pattern
Theme management allows runtime strategy selection:
- Different theme strategies (dark/light)
- Runtime switching without code changes
- Extensible to new themes

### 5. Dependency Injection
Constructor injection pattern:

```c
MainWindow* main_window_create(Application* app);
```

Dependencies are injected, not created internally, enabling:
- Testability
- Flexibility
- Loose coupling

## Memory Management

### Ownership Rules

1. **Creator Owns**: The module that creates an object owns it and must destroy it
2. **String Ownership**:
   - Functions returning `const char*` - caller does NOT free
   - Functions returning `char*` - caller MUST free
   - Functions accepting `const char*` - make internal copy if needed

### Resource Lifecycle

```c
// Application lifecycle
Application* app = application_create();  // Allocates all subsystems
// ... use application ...
application_destroy(app);                  // Frees all subsystems

// Document lifecycle (managed by Application)
app->document = document_create();
// ... 
document_destroy(app->document);

// UI lifecycle
MainWindow* window = main_window_create(app);
// ...
main_window_destroy(window);              // GTK widgets auto-destroyed
```

### Memory Safety

- All allocations checked for NULL
- Defensive programming (NULL checks before use)
- Consistent cleanup patterns
- No circular references

## Error Handling

### Strategy
1. **Return codes** for operations that can fail
2. **NULL returns** for allocation failures
3. **Callbacks** for asynchronous error reporting
4. **User-friendly messages** for UI errors

### Error Propagation

```c
// Low level - detailed error codes
FileOperationResult result = file_operations_read(path, doc);

// Mid level - boolean + callback
if (result != FILE_OP_SUCCESS) {
    if (app->callbacks.on_error) {
        app->callbacks.on_error(error_message, user_data);
    }
    return false;
}

// High level - user dialog
static void on_error(const char* message, void* user_data) {
    MainWindow* window = (MainWindow*)user_data;
    main_window_show_error(window, message);
}
```

## Threading Model

### Current Implementation
Single-threaded with GTK main loop:
- All operations on main thread
- No explicit thread management needed
- GTK handles event dispatch

### Future Considerations
For long-running operations:
- Background threads for file I/O
- Progress callbacks
- Thread-safe document access

## Extensibility Points

### Adding New Themes
1. Add theme colors to `theme_manager.c`:
   ```c
   static const ThemeColors new_theme = { ... };
   ```
2. Add CSS definition:
   ```c
   static const char* new_theme_css = "...";
   ```
3. Extend `ThemeType` enum
4. Update `theme_manager_get_colors()` and `theme_manager_get_css()`

### Adding New File Formats
1. Extend `FileOperationResult` enum if needed
2. Add format-specific read/write functions
3. Update `file_operations_read()` with format detection
4. Maintain existing interface

### Adding New Features
1. **Undo/Redo**:
   - Add `history` module in `core/`
   - Implement command pattern
   - Integrate with `Document`

2. **Find/Replace**:
   - Add `search` module in `core/`
   - Create search dialog in `ui/`
   - Coordinate through `Application`

## Testing Strategy

### Unit Testing
Each module can be tested independently:

```c
// Example: Testing document
void test_document_create(void) {
    Document* doc = document_create();
    assert(doc != NULL);
    assert(!document_is_modified(doc));
    document_destroy(doc);
}
```

### Integration Testing
Test module interactions:

```c
// Example: Testing file operations with document
void test_file_save_load(void) {
    Document* doc = document_create();
    document_set_content(doc, "Test content");
    file_operations_write("test.txt", doc);
    
    Document* doc2 = document_create();
    file_operations_read("test.txt", doc2);
    assert(strcmp(document_get_content(doc2), "Test content") == 0);
    
    document_destroy(doc);
    document_destroy(doc2);
}
```

### Manual Testing
- UI interaction flows
- Edge cases (empty files, large files)
- Error conditions (permission denied, disk full)

## Build System

### Makefile Structure
- Modular object file compilation
- Automatic dependency generation
- Debug and release configurations
- Clean separation of build artifacts

### Dependency Management
- `pkg-config` for GTK dependencies
- Header file dependencies tracked
- Incremental builds supported

## Code Style Guidelines

### Naming Conventions
- **Functions**: `module_verb_object()` (e.g., `document_set_content()`)
- **Types**: `PascalCase` for structs (e.g., `Document`, `ThemeManager`)
- **Variables**: `snake_case` (e.g., `file_path`, `user_data`)
- **Constants**: `UPPER_SNAKE_CASE` (e.g., `FILE_OP_SUCCESS`)

### File Organization
- One module per file pair (.h/.c)
- Headers in `include/`, sources in `src/`
- Mirror directory structure
- Forward declarations in headers

### Documentation
- Doxygen-style comments in headers
- Implementation comments for complex logic
- File-level documentation
- Function contracts clearly specified

## Performance Considerations

### Current Optimizations
- Single buffer allocation for file reading
- Efficient string handling
- Minimal GTK widget creation

### Scalability
Current limits:
- File size limited by available memory
- Single document at a time
- No lazy loading

Future improvements:
- Memory-mapped files for large documents
- Piece table or rope data structure
- Chunked loading

## Security Considerations

### File Operations
- Path validation
- Permission checking before operations
- Safe string handling (bounded copies)
- No shell command execution

### Input Validation
- NULL pointer checks
- Buffer size validation
- File path sanitization

## Conclusion

This architecture provides:
- **Maintainability**: Clear separation of concerns
- **Testability**: Independent, testable modules
- **Extensibility**: Easy to add new features
- **Reliability**: Robust error handling
- **Performance**: Efficient resource usage

The design balances simplicity with professional practices, making it an excellent example of well-architected C code.