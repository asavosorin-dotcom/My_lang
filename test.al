proto print ;

func sum (var a, var c, var d)
{
	var num1 = a + c;
	num1 = num1 + d;
	return num1;
};

func _start()
{
	var c = sum(12, 13, 15);
	print(c);
	print(10);
	print(11);
};
