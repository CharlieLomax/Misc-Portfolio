#pragma once
#include "duktape.h"
#include <string>
#include <vector>
#include <iostream>

// Provides a wrapper for the context, as well as some helper functions
class ScriptManager
{
	duk_context* ctx;

	bool scriptLoaded;

public:

	// Dynamically allocates the context
	ScriptManager();

	// Since we are dynamically allocating memory in the constructor, we should deallocate it in the destructor.
	~ScriptManager();

	// Pursuant to Rule of Three, a copy constructor is explicitly defined
	ScriptManager(const ScriptManager& obj);

	// Pursuant to Rule of Three, a copy operator is explicitly defined
	ScriptManager& operator=(const ScriptManager& obj);

	// Returns a the context being wrapper around
	duk_context* getContext();

	// Reads in the file with the given name, and puts its contents on the stack
	void loadScript(const std::string fileName);

	// Defines which function from the loaded file is to be run, how many arguments it takes, and what those arguments are
	template <typename... Args>
	void runScript(std::string functionName, int arg_count = 0, Args... args)
	{
		if (this->scriptLoaded)
		{
			// This indicates the name of the function within the script file that you wish to run
			duk_get_prop_string(ctx, -1, functionName.c_str());

			// This pushes all the passed in arguments onto the stack
			dukglue_push(this->ctx, args...);

			//If you want your script function to return non-string values, or operate on/do something with return values, you will have to modify this
			if (duk_pcall(ctx, arg_count) != 0)
			{
				std::cout << "Error: " << duk_safe_to_string(ctx, -1) << std::endl;
			}
			//std::cout << duk_safe_to_string(ctx, -1) << std::endl;
			duk_pop(this->ctx);
		}
		else
		{
			std::cout << "ERROR: Cannot run script unless script is loaded, function is chosen, and arguments are given" << std::endl;
			exit(1);
		}

	}

	


};

