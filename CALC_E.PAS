program calc_e;

uses StrMath;

var e:TStrMath;
    term:TStrMath;
    n:Longint;

begin
  e.Init;
  e.digits[0]:=1;
  term.init;
  term.digits[0]:=1;
  for n:=1 to 100 do begin
    term.dividebyint(n);
    e.addto(term);
  end;
  e.print;
  writeln;
end.