int
bar(int param1 , int param2)
{
  int result;
  result = param1 * 11;
  result = param1 + param2;
  return result;
}

int
foo()
{
  int local1;
  int local2;
  int local3;
  local1 = bar(42, 17);
  local2 = local1 / 5;
  local3 = local2 / 7;
  return local3;
}

int
notmain ( void )
{
  int code;
  code = foo();
  return code;
}
