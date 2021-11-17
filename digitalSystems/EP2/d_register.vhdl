-------------------------------------------------------
--! @file d_register.vhdl
--! @brief Registrador para EP2 da disciplina de Sistemas Digitais I
--! @author Igor Pontes Tresolavy (tresolavy@usp.br)
--! @date 2021-11-03
-------------------------------------------------------

library IEEE;
use IEEE.numeric_bit.all;

entity d_register is
    generic(
        width: natural := 4;
        reset_value: natural := 0
    );
    port(
        clock, reset, load: in bit;
        d: in bit_vector(width-1 downto 0);
        q: out bit_vector(width-1 downto 0)
    );
end d_register;

architecture memory of d_register is

        -- value of q port when reset is on
        constant uns_reset_value: unsigned(width-1 downto 0) := to_unsigned(reset_value, width);

    begin

        D_ff: process(clock, reset) is
            begin
                if reset = '1' then
                    q <= bit_vector(uns_reset_value);
                elsif load = '1' and clock'event and clock = '1' then
                    q <= d;
                end if;
        end process D_ff;

end architecture memory;