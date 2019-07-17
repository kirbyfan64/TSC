# -*- coding: utf-8 -*-

module Std

  ##
  # Class: Std::GiantJewelBox
  #
  # This class fills a box with any amount of jewels. When the player
  # activates the box, the jewels are thrown out of the box all at once
  # and move over the field. Use it like this:
  #
  # <pre class="source">
  # b = GiantJewelBox.new(UIDS[14], 10)
  # </pre>
  #
  # This fills the box with UID 14 with 10 jewels of random
  # nature (red or blue ones).
  class GiantJewelBox

    # The underlying Box instance
    attr_reader :box

    # The number of jewels to create.
    attr_accessor :count

    ##
    # Method: Std::GiantJewelBox::new
    #
    #   new(box, count) → a_giant_jewel_box
    #
    # Creates a new giant jewel box.
    #
    # <h4>Parameters</h4>
    # <dl>
    # <dt>box</dt><dd>A Box instance to attach the climbing plant to. May also be a raw UID.</dd>
    # <dt>count</dt><dd>The number of jewels to place in the box.</dd>
    # </dl>
    def initialize(box, count)
      @box = box.kind_of?(Integer) ? UIDS[box] : box
      @count = count
      @activated = false
    end

    ##
    # Method: Std::GiantJewelBox#attach
    #
    #   attach()
    #
    # Fill the box with jewels.
    def attach
      @box.on_activate do
        unless @activated
          Audio.play_sound("item/jewel_2.ogg")
          @count.times do
            jewel = FallingJewel.new
            jewel.start_at(box.x, box.y - 30)
            jewel.gold_color = rand(10) <= 7 ? :yellow : :red
            jewel.velocity_y = rand(61)
            jewel.velocity_x = -10 + rand(21)
            jewel.show
          end
          @activated = true
        end
      end

      # Automatic saving and loading.
      Level.on_save do |store|
        store["_ssl"] ||= {}
        store["_ssl"]["giant_jewel_boxes"] ||= {}
        store["_ssl"]["giant_jewel_boxes"][@box.uid] = @activated
      end

      Level.on_load do |store|
        if store["_ssl"] && store["_ssl"]["giant_jewel_boxes"] && store["_ssl"]["giant_jewel_boxes"][@box.uid]
          @activated = store["_ssl"]["giant_jewel_boxes"][@box.uid]
        end
      end
    end

    ##
    # Method: Std::GiantJewelBox#activated?
    #
    #   activated?() → bool
    #
    # Has the box been activated?
    def activated?
      @activated
    end

  end

end
