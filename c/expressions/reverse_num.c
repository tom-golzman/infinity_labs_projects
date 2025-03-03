int reverse (int num)
{
	int newNum = 0;

	while (num != 0)
	{
		newNum = newNum*10 + (num%10);
		num /= 10;
	}
	
	return newNum;
}

int main ()
{
	return 0;
}

