# README

<figure><img src="https://catalystdigitalstudio.xyz/assets/icon.png" alt="Catalyst"><figcaption></figcaption></figure>

## Catalyst Digital Studio

### Syntax

Catalyst uses the [CamelCase ](https://en.wikipedia.org/wiki/Camel\_case)naming scheme.

#### Methods

Methods use lower [camelCase](https://en.wikipedia.org/wiki/Camel\_case).

#### Variables

Variables use a prefix with lower [camelCase](https://en.wikipedia.org/wiki/Camel\_case).

* Member variables have the prefix `m_`
* Data structure variables have no prefix

### Getting Started

## [IApplication](classes/iapplication.md)

#### Example.h

```cpp
// Exmaple.h

#include "Catalyst.h"

class ExampleApplication : public Catalyst::IApplication
{

public:
    ExampleApplication();
    ~ExampleApplication() = default;
    
    virtual void onRun() override;

};
```

The main function we should look at right now is [`IApplication::onRun()`](classes/iapplication.md#onrun), this function is called by the Catalyst backend to start your application.

#### Example.cpp

```cpp
// Example.cpp

#define CATALYST_MAIN

#include "Example.h"

CATALYST_LAUNCH(ExampleApplication);

ExampleApplication::ExampleApplication
    : Catalyst::IApplication("Example Application")
{

}

void ExampleApplication::onRun()
{

}
```

This is the basic application layout. In the constructor of your application give [`IApplication::IApplication(const char*)`](classes/iapplication.md#iapplication) your application name. [`onRun()`](classes/iapplication.md#onrun) should contain your main application loop. This application will be used to manage your scenes.



## The Loop

#### Lifetime

```cpp
void ExampleApplication::onRun()
{
    
    while(! close())
    {
        //Do stuff
    }
    
}
```

Here we use the `close()` method to check if the application should close.

```cpp
void ExampleApplication::onRun()
{
    
    close(true);
    
    while(! close())
    {
        //Do stuff
    }
    
}
```

Next, we can signal the application to close by using the same `close()` function. However, instead of getting the return of the function we can provide it with a boolean.


