library IEEE;
use IEEE.std_logic_1164.all;

entity functions1 is
port(
        A   : in std_logic;
        B   : in std_logic;

        F   : out std_logic
);
end entity functions1;

architecture first of functions1 is

begin

    first_proc: process(A, B)

    begin
        if A = '1' then
            F <= '1';
        elsif B = '1' then
            F <= '1';
        else
            F <= '0';
        end if;

    end process first_proc;

end architecture first;

library IEEE;
use IEEE.std_logic_1164.all;

entity functions2 is
  port (
    A   : in std_logic;
    B   : in std_logic;
    C   : in std_logic;
    D   : in std_logic;

    F   : out std_logic
  ) ;
end entity functions2 ;

architecture second of functions2 is

begin

    second_proc: process( A, B, C, D )
    begin
        if B = '0' AND C = '1' then
            F <= '1';
        elsif D = '1' OR C = '0' then
            F <= '0';
        else
            F <= '1';
        end if ;
    end process second_proc; 

end architecture second;

library IEEE;
use IEEE.std_logic_1164.all;

entity functions3 is
  port (
    A   : in std_logic;
    B   : in std_logic;
    C   : in std_logic;
    D   : in std_logic;

    F   : out std_logic
  ) ;
end entity functions3 ;

architecture third of functions3 is

    signal prod  : std_logic_vector(2 downto 0);

begin
    prod(0) <= (NOT A) OR B;
    prod(1) <= (NOT B) OR C OR D;
    prod(2) <= (NOT A) OR D;

    third_proc: process (prod)
    begin
        if prod = "111" then
            F <= '1';
        else
            F <= '0';
        end if ;

    end process third_proc;

end architecture third; 
