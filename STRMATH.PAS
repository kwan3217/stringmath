Unit StrMath;

interface

const N:Word=50; { Number of digits }
      k:Word=10;

type TStrMath=object
  Digits:array[0..4999] of Byte;
  constructor Init;
  procedure DivideByInt(d:LongInt);
  procedure AddTo(b:TStrMath);
  procedure print;
end;

implementation

constructor TStrMath.Init;
  var i:word;
begin
  for i:=0 to N-1 do begin
    Digits[i]:=0;
  end;
end;

procedure TStrMath.DivideByInt;
  var r:LongInt;
      i:Word;
      temp:LongInt;
begin
  if d=0 then begin
    {d:=1 div 0; {Cause an integer divide-by-zero}
  end;
  r:=0;
  for i:=0 to N-1 do begin
    temp:=r*k+Digits[i];
    r:=temp mod d;
    temp:=temp div d;
    Digits[i]:=temp;
  end;
end;

procedure TStrMath.AddTo;
  var i:Word;
      c:Byte;
begin
  c:=0;
  for i:=N-1 downto 0 do begin
    c:=digits[i]+b.digits[i]+c;
    digits[i]:=c mod k;
    c:=c div k;
  end;
end;

procedure TStrMath.print;
  var i:Word;
begin
  write(Digits[0],'.');
  for i:=1 to N-1 do begin
    write(Digits[i]);
  end;
end;




end.