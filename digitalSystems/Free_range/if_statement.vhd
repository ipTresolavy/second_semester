-- library declaration
library IEEE;
use IEEE.std_logic_1164.all;


entity if_statement is
port(
    testIn      : in std_logic;
    
    testOut     : out std_logic     
);
end if_statement;

architecture if_arch of if_statement is
begin
    if_proc: process(testIn) is
    begin
        if testIn = '1' then
            testOut <= '0';
        else
            testOut <= '1';
        end if;
    end process if_proc;
end if_arch;