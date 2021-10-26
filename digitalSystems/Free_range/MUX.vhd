-- library declaration
library IEEE;
use IEEE.std_logic_1164.all;

entity MUX is
port(
    Data_in     : in std_logic_vector(7 downto 0);
    SEL         : in std_logic_vector(2 downto 0);
    CE          : in std_logic;

    F_CTRL      : out std_logic
);
end MUX;

architecture MUX_arch of MUX is
begin
    my_mux: process(SEL, Data_in, CE)
    begin
    if CE = '1' then
        case SEL is
            when "000" => F_CTRL <= Data_in(0); 
            when "001" => F_CTRL <= Data_in(1);
            when "010" => F_CTRL <= Data_in(2);
            when "011" => F_CTRL <= Data_in(3);
            when "100" => F_CTRL <= Data_in(4);
            when "101" => F_CTRL <= Data_in(5);
            when "110" => F_CTRL <= Data_in(6);
            when others => F_CTRL <= Data_in(7);
        end case;
    end if;
    end process my_mux;
end MUX_arch;
