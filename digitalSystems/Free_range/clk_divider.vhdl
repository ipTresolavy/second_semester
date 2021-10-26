library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;

entity clk_divider is
port(
    clk     : in std_logic;
    div_en  : in std_logic;

    sclk    : out std_logic
);
end entity clk_divider;

architecture clk_div_arch of clk_divider is

    type rate is range 0 to 64;

    constant max    : rate := 64;

    signal tmp_clk  : std_logic;


begin

    tmp_clk <= clk; 

    clk_div: process(div_en, clk)
    variable div  : rate := 0;
    begin
        if div_en = '1' then
            if div = max then
                div := div + 1;
            else
                tmp_clk <= not tmp_clk;
                div := 0;
            end if;
        else
            tmp_clk <= '0';
            div := 0;
        end if;
    end process clk_div; 

    sclk <= tmp_clk;

end architecture clk_div_arch;