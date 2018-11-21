# SimpleArgParser

SimpleArgParser is a minimalistic command-line argument parser for C++, inspired by other similar libraries, mainly the Python argparse module. Although there are a lot of similiar libraries this one focus on being over-simplified, by limiting the functionalities to the bare minimum necessary for it to work. The main features of it are:

* Include-Only: meaning that no linking is needed
* Object-oriented: so people can customize the parser in sub-classes
* Simplified interface: the reduced number of functions fits very well in the user brain cache! ;)

Please note that this is a very simple library, so if you need a more robust library with more tools and option this may not be for you. If that's the case there are other libraries that are also simple but more robust than SimpleArgParser.

## Usability ( in progress :p )

To use SimpleArgParser you need basically 3 steps:

	1. Initialization and setup of the class instance, which includes the definition of the expected arguments;
	2. Parsing, that is just a function call: just remember to catch the exception
	3. Retrieve the results with the power of templates


### Step 1: Setup

So for step 1, all you need is to pass all the command-line args to this beauty (parser::ArgParser) in the constructor, like in:

```
parser::ArgumentParser argParser(argc, argv);
```

And then include your arguments:

```
// Including a new flag with no value to parse via command line
argParser.addArgument(

    /*argument name*/ 
    "--Yaay",

    /*argument will contain a value?*/ 
    false, 			

    /*argument description*/
    "Mass of that thing used by that other thing"
);

// Including argument with value
argParser.addArgument(
    "--mass", 
    true, 
    "Mass of that thing used by that other thing");

```
### Step 2: Parse!

Parse:

```
argParser.parse();
```

### Step 3: Retrieve!

You have 3 main methods to interact with the parsed results. In the examples we will use the 2 arguments defined above. They are `isDefined`, `hasValue` and `getArgument`.

#### 3.1: isDefined

Returns a boolean indicating that the argument was passed as a command-line argument. So if you call the program like this:

```
superCoolSoftware --Yaay
```

Then this:

```
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

```
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

```
auto mass = argParser.getArgument<int>("--mass");
auto mass = argParser.getArgument<float>("--mass");
auto mass = argParser.getArgument<double>("--mass");
auto mass = argParser.getArgument<std::string>("--mass");
```
And you'll get the type you asked for (if the string is in the format).

P.S.: Currently the library don't verify if the string meets the format requirements for the type you ask, so be careful until then. Or if you're feeling generous, you can contribute with an issue/PR. 