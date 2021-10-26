library IEEE;
use IEEE.std_logic_1164.all;

entity two_one_MUX is
port(
    D1_IN, D2_IN    : in std_logic_vector(7 downto 0);
    SEL             : in std_logic;
    
    MUX_OUT         : out std_logic_vector(7 downto 0)
);
end entity two_one_MUX;

architecture two_one_MUX_arch of two_one_MUX is
begin
    MUX_prox : process( SEL, D1_IN, D2_IN )
    begin
        if SEL = '0' then
            MUX_OUT <= D1_IN;
        else
            MUX_OUT <= D2_IN;
        end if;
    end process MUX_prox;
end two_one_MUX_arch;