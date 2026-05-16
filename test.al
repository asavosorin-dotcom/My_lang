func sum (var a, var c, var d)
{
	while (a > 0)
	{
		var sumint = 0;
		
		while (a > 0)
		{
			sumint = sumint + a;
			a = a - 1;
		};

		sumint = sumint + c + d;
	};

	return sumint;
};

func _start()
{
	var c = sum(9, 7, 13);
	print(c);
};
