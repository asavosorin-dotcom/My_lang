proto print ;

func sum (var a, var c, var d)
{
	var num1 = a / c;
	num1 = num1 / d;
	return num1;
};

func _start()
{
	var c = sum(0 - 10,0 - 4,0 - 2);
	print(c); 
	print(10);
	print(11);
};
