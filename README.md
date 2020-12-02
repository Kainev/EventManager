
<h1 align="center"> Event Manager </h1><br>
Single-header C++ Event System.

## Contents
- [Aim](#aim)
- [Future](#future)
- [Requirements](#requirements)
- [API](#api)
	- [Registering Function Callbacks](#registering-function-callbacks)
	- [Registering Member Callbacks](#registering-member-callbacks)
	- [Unregister Callbacks](#deregister-callbacks)
- [Installation](#installation)

## Aim

 - Easy to use external API
	 - Simple, intuitive syntax
	 - No binding or passing in lambdas required
 - Events are simple PODS
	 - No inheritance of a base `Event` class
	 - No explicit registering of events
 - Efficient operation
	 - Suitable for real-time applications
	 
## Future
 - Delayed event queues
	 - Fired events can be pushed to a queue for deferred execution
 - Asynchronous events

## Requirements
- C++17

## API
### Registering Function Callbacks
```cpp
#include "EventManager.h"

const int key_w = 87;

struct KeyPressEvent
{
    int key_code;
}

void on_key_press(KeyPressEvent* event)
{
    // ...
}

int main()
{
    EventManager::listen<KeyPressEvent>(&on_key_press);
    EventManager::fire<KeyPressEvent>(key_w);
}
```

### Registering Member Callbacks
```cpp
#include "EventManager.h"

const int key_w = 87;

struct KeyPressEvent
{
    int key_code;
}

class InputHandler
{
public:
    InputHandler()
    {
        EventManager::listen<KeyPressEvent>(this, &InputHandler::on_key_press);
    }
private:
    void on_key_press(KeyPressEvent* event)
    {
         // ...
    }
}

int main()
{
    InputHandler input_handler;
    EventManager::fire<KeyPressEvent>(key_w);
}
```

### Unregister Callbacks
A `ListenerHandle` is returned to the user upon registering an event callback, and each callback function is uniquely associated with its given handle. 
```cpp
#include "EventManager.h"

const int key_w = 87;

struct KeyPressEvent
{
    int key_code;
}

void on_key_press(KeyPressEvent* event)
{
    // ...
}

int main()
{
    ListenerHandle handle = EventManager::listen<KeyPressEvent>(&on_key_press);
    EventManager::remove_listener(handle);
}
```

## Installation
Event Manager is a single-header library so no build is required. You can either clone or directly download the repository and place `EventManager.h` in your include folder.

From here it is as simple as `#include "EventManager.h"` and you're ready to go.
