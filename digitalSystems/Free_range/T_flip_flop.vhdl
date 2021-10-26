library IEEE;
use IEEE.std_logic_1164.all;

entity tff is
port(
    T   : in std_logic;
    clk : in std_logic;
    set : in std_logic;

    Q   : out std_logic
);
end entity tff;

architecture tff_arch of tff is
    signal temp : std_logic;
begin
    tff_proc: process (T, clk, set)
    begin
        if set = '0' then
            temp <= '1';
        elsif rising_edge(clk) then
            temp <= T XOR temp;
        end if;
    end process tff_proc;
    
    Q <= temp;
end architecture tff_arch;