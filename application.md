# Application

## Class

**IApplication -** Your application should implement the **IApplication** interface.

```cpp
//ExampleApplication.h

#include "Catalyst.h"

class ExampleApplication : public Catalyst::IApplication
{

public:
    ExampleApplication() = default;
    ~ExampleApplication() = default;
    
    virtual void onRun() override;
    
};
```

```cpp
//ExampleApplication.cpp

#include "ExampleApplication.h"

void ExampleApplicaiton::onRun()
{
    while(!close())
    {
         close(true);
    }
}
```

