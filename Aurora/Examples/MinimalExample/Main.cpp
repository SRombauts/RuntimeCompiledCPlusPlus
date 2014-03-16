#include <iostream>

#include "../../RuntimeObjectSystem/RuntimeObjectSystem.h"
#include "../../RuntimeObjectSystem/IObjectFactorySystem.h"


class MainApp : public IObjectFactoryListener
{
public:
    MainApp()
    {
        // TODO Returns always true!
        mRuntimeObjectSystem.Initialise (nullptr, nullptr);
        
        mRuntimeObjectSystem.GetObjectFactorySystem()->AddListener(this);
    }
    ~MainApp()
    {
        mRuntimeObjectSystem.CleanObjectFiles();
    }

	// Called after a full serialization of objects is done when a new object constructor is added,
    // so listeners can update any object pointers they're holding
	virtual void OnConstructorsAdded() override
    {
        std::cout << "OnConstructorsAdded()\n";
    }

    void Loop()
    {
        static int _nbLoop = 0;
        while (true)
        {
            ++_nbLoop;
	        // check status of any compile
	        if (mRuntimeObjectSystem.GetIsCompiledComplete())
	        {
                std::cout << _nbLoop << ": CompiledComplete: LoadCompiledModule()\n";
		        // load module when compile complete
		        mRuntimeObjectSystem.LoadCompiledModule();
	        }
            else
            {
	            if (mRuntimeObjectSystem.GetIsCompiling())
	            {
                    std::cout << _nbLoop << ": Compiling...\n";
                }
                else
	            {
                    std::cout << _nbLoop << ": No compilation.\n";
                }
            }

            float deltaTime = 1.0f;
            // TODO Why using pointers if never NULL?
    		mRuntimeObjectSystem.GetFileChangeNotifier()->Update(deltaTime);
            Sleep(static_cast<DWORD>(deltaTime*1000));
        }        
    }

private:
    RuntimeObjectSystem mRuntimeObjectSystem;
};


int main(int argc, char* argv[])
{
    std::cout << "Starting...\n";

    MainApp app;
    app.Loop ();

    std::cout << "Exiting...\n";
	return 0;
}