# SimpleArgParser

SimpleArgParser is a minimalistic command-line argument parser for C++, inspired by other similar libraries, mainly the Python argparse module. Although there are a lot of similiar libraries this one focus on being over-simplified, by limiting the functionalities to the bare minimum necessary for it to work. The main features of it are:

* Include-Only: meaning that no linking is needed
* Object-oriented: so people can customize the parser in sub-classes
* Simplified interface: the reduced number of functions fits very well in the user brain cache! ;)

Please note that this is a very simple library, so if you need a more robust library with more tools and option this may not be for you. If that's the case there are other libraries that are also simple but more robust than SimpleArgParser.

## Usability

To use SimpleArgParser you need basically 3 steps:

1. Initialization and setup of the class instance, which includes the definition of the expected arguments;
2. Parsing, that is just a function call: just remember to catch the exception
3. Retrieve the results with the power of templates


### Step 1: Setup

So for step 1, all you need is to pass all the command-line args to this beauty (parser::ArgumentParser) in the constructor, like in:

```
parser::ArgumentParser argParser(argc, argv);
```

And then include your arguments:

```cpp
// Including a new flag with no value to parse via command line
argParser.addArgument(

    /*argument name*/ 
    "--Yaay",

    /*argument will contain a value?*/ 
    false, 			

    /*argument description*/
    "Say Yaay!"
);

// Including argument with value
argParser.addArgument(
    "--mass", 
    true, 
    "Mass of that thing used by that other thing");

```
#### 1.1 Usage

You can, at this point, call the method `argParser.usage()`, that returns a string with the software usage, like:

```
Usage:
    ./a.out [ --Yaay ]  [ --mass val]  

    [ --Yaay ] => Say Yaay!

    [ --mass val] => Mass of that thing used by that other thing

```


### Step 2: Parse!

Parse:

```cpp
argParser.parse();
```

#### 2.1 Beware the exception

When parsing, the class will check if an argument is expected. If not, it will throw an `parser::parsing_exception`. This exception can be as informative as you like by using one of its 3 methods: `what()`, `usage()` and `message()`. Let's suppose you call the program like:

```
superCoolSoftware --test
```

Since we haven't defined '--test' yet, the `argParser.parse()` will throw the exception. Between you and me, I know that you are a very careful developer and will not let this exception go under your nose, so you can do this:

```cpp
try{
    argParser.parse();
} 
catch(parser::parsing_exception& e)
{
    // Will print a string with the cause of the error.
    std::cout << e.what() << "\n\n";
    // Will print the usage of the software.
    // Same content as calling argParser.usage()
    std::cout << e.usage() << "\n\n";
    // Will merge the two strings above together in a
    // nice and informative message, for the ones who 
    // don't want to format strings
    std::cout << e.message() << "\n\n";
}
```

### Step 3: Retrieve!

You have 3 main methods to interact with the parsed results. In the examples we will use the 2 arguments defined above. They are `isDefined`, `hasValue` and `getArgument`.

#### 3.1: isDefined

Returns a boolean indicating that the argument was passed as a command-line argument. So if you call the program like this:

```
superCoolSoftware --Yaay
```

Then this:

```cpp
if(argParser.isDefined("--Yaay"))
	std::cout << "Yaay!" << std::endl;
```

Will print:

```
Yaay!
```

#### 3.2: hasValue

This one indicates if a EXPECTED AND DEFINED command-line argument contains a value to be used. So calling:

```
superCoolSoftware --mass 12.031
```

Then this:

```cpp
if(argParser.hasValue("--mass"))
	std::cout << "Mass is heavy" << std::endl;
else
	std::cout << "We are weightless" << std::endl;
```
Will print:

```
Mass is heavy
```

But if we call the program like:

```
superCoolSoftware
```

Then we'll have:

```
We are weightless
```

#### 3.3 getArgument

The last method retrieve the value of the defined and expected arguments. The method uses templates to get the value in the basic type you need, so you can call:

```cpp
auto mass = argParser.getArgument<int>("--mass");
auto mass = argParser.getArgument<float>("--mass");
auto mass = argParser.getArgument<double>("--mass");
auto mass = argParser.getArgument<std::string>("--mass");
```
And you'll get the type you asked for (if the string is in the correct format).

P.S.: Currently the library don't verify if the string meets the format requirements for the type you ask (i.e.: "text" as `float`), so be careful until then. Or if you're feeling generous, you can contribute with an issue/PR. 