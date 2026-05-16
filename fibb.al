proto print ;

func fibb (var n)
{
	if (n == 0)
	{
		return 0;
	};

	if (n == 1)
	{
		return 1;
	};

	var n1 = n - 1;
	var n2 = n - 2;

	var f1 = fibb(n1);
	var f2 = fibb(n2);

	var fsum = f1 + f2;

	return fsum;
};

func _start()
{
	var f0 = fibb(10);
	print(f0);
};
