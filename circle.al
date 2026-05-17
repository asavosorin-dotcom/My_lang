proto printchar ;

func _start()
{
	var xmax = 51;
	var ymax = 51;
	var r    = 15;

	var x = 0;
	var y = 0;

	var inc = 0;

	while (y < ymax)
	{
	    x = 0;
	    while (x < xmax)
	    {
		if ((x - 20) * (x - 20) + 4 * (y - 20)*(y - 20) < r*r)
		{
		    printchar(42);
		};

		if ((x - 20) * (x - 20) + 4 * (y - 20) * (y - 20) >= r*r)
		{
		    printchar(46);
		};

		x = x + 1;
	    };
	    printchar(10);

	    y = y + 1;
	};
};
