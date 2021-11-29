-------------------------------------------------------
--! @file rom_simples.vhdl
--! @brief Implementação de ROM simples em VHDL para EP3 da disciplina de Sistemas Digitais I
--! @author Igor Pontes Tresolavy (tresolavy@usp.br)
--! @date 2021-11-28
-------------------------------------------------------

library IEEE;
use IEEE.numeric_bit.all;

entity rom_simples is
    port (
        addr: in bit_vector(3 downto 0);
        data: out bit_vector(7 downto 0)
    );
end entity rom_simples;

architecture read_only_memory of rom_simples is

    type mem_tipo is array(0 to 15) of bit_vector(7 downto 0 );

    constant mem: mem_tipo :=
    (
        0  => "00000000",
        1  => "00000011",
        2  => "11000000",
        3  => "00001100",
        4  => "00110000",
        5  => "01010101",
        6  => "10101010",
        7  => "11111111",
        8  => "11100000",
        9  => "11100111",
        10 => "00000111",
        11 => "00011000",
        12 => "11000011",
        13 => "00111100",
        14 => "11110000",
        15 => "00001111"
    );

    begin

        data <= mem(to_integer(unsigned(addr)));

end architecture read_only_memory;