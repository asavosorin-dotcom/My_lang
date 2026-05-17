proto print ;
proto scan  ;

func factorial (var x)
{
    if (x != 1)
    {
        var b = x - 1;
        var c = x - 2;
        var f = 0;
        f = factorial(b) * x;
        return f;
    };
    
    return 1;
};

func _start()
{
	var f = scan(); 
	var c = factorial(f);
	print(c);
};
