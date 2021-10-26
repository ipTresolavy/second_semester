library IEEE;
use IEEE.std_logic_1164.all;

entity D_flip_flop is
port(
        D : in std_logic;
        clk : in std_logic;

        Q   : out std_logic
);

end entity D_flip_flop;


architecture flip_flop of D_flip_flop is
begin

    flip_flop_proc: process (clk)
    begin
        if clk'event AND clk = '1' then
            Q <= D;
        end if;
    end process flip_flop_proc;    

end architecture flip_flop;