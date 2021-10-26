library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
 
entity up_down_counter is
port( 
    clock : in std_logic;
    reset : in std_logic;
    up_down : in std_logic;
    counter : out std_logic_vector(3 downto 0));

end up_down_counter;
 
architecture bhv of up_down_counter is
signal t_count: std_logic_vector(3 downto 0);
begin
    process (clock, reset)
    begin
        if (reset='1') then
            t_count <= "0000";
        elsif rising_edge(clock) then
            if up_down = '0' then
                t_count <= t_count + 1;
            else
                t_count <= t_count - 1;
            end if;
        end if;
    end process;
 
    counter <= t_count;
end bhv;