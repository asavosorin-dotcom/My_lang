proto print ;
proto scan  ;

func sum (var a, var c, var d)
{
	var num1 = a / d;
	num1 = num1 * c;
	return num1;
};

func _start()
{
	var x = scan();
	var sqr = sqrt(x);
	print(sqr);

	print(10);
	print(11);
};
