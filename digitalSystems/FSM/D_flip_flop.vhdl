library IEEE;
use IEEE.std_logic_1164.all;

entity D_flip_flop is
generic (
    initial: std_logic 
);
port(
    D       : in std_logic;
    clk     : in std_logic;

    Q, Qinv : out std_logic
);
end entity D_flip_flop;

architecture flip_flop of D_flip_flop is
    
    signal state : std_logic := initial;

begin

    Q <= state;
    Qinv <= not state;

    Dff: process (clk)
    begin
        if rising_edge(clk) then
            state <= D;
        end if;
    end process Dff;
    
end architecture flip_flop;