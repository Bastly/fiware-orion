# -*- coding: utf-8 -*-
# Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
#
# This file is part of Orion Context Broker.
#
# Orion Context Broker is free software: you can redistribute it and/or
# modify it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
#
# Orion Context Broker is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
# General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
#
# For those usages not covered by this license please contact with
# iot_support at tid dot es

__author__ = 'Jon Calderin Goñi (jcaldering@gmail.com)'

# Created by Jon at 10/02/2015
Feature: ServicePath for registrations (NGSI9) (not recursive)
  # Enter feature description here
  @issue-719
  Scenario: Subscribe different entities with the same provider and services but different subservice
    # First registration
    Given a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
      | Room2     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application |
      | /service1             |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
      | Room3     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application |
      | /service2             |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Discover
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type | is_pattern |
      | Room.*    | Room        | true       |
    And build the standard discover context availability payload with the previous data
    When a standard disconver context availability is asked with the before information
    Then check the response has the key "id" with the value "Room2"
    And check the response has not the key "id" with the value "Room3"
    And clean the mongo database of the service "issue_719"

  @issue-719
  Scenario: Query context to a subservice
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1/1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1/2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Query consult
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice1/2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard query context payload with the previous data
    When a standard query context is asked with the before information
    #Mock information
    Then retrieve information from the mock
    And the path in the last mock petition contains "service2"
    And there is "1" petitions requested to the mock
    And  clean the mongo database of the service "issue_719"

  @issue-719
  Scenario: Update an entity in a context provider, having other context provider with the same entity in other subservice
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Update operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice1"
    And the following attributes to create
      | attribute_name | attribute_type | attribute_value |
      | att1           | att_type_1     | 25              |
    And a context elements with the before attrs and the following entities
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard entity update payload with the previous data
    When a standard context entity update is asked with the before information
    Then retrieve information from the mock
    And the path in the last mock petition contains "service1"
    And there is "1" petitions requested to the mock
    And clean the mongo database of the service "issue_719"

  @issue-719
  Scenario: Append an entity in a context broker, having context providers with the same entity in the same subservice and other in other subservice
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Append operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice1"
    And the following attributes to create
      | attribute_name | attribute_type | attribute_value |
      | att1           | att_type_1     | 25              |
    And a context elements with the before attrs and the following entities
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard entity creation payload with the previous data
    When a standard context entity creation is asked with the before information
    Then retrieve information from the mock
    And there is "0" petitions requested to the mock
    And clean the mongo database of the service "issue_719"

  @issue_755 @issue-719
  Scenario: Delete an entity doesnt exist in context broker (error expected), having context providers with the same entity in the same subservice and other in other subservice
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Delete operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice1"
    And the following attributes to create
      | attribute_name | attribute_type | attribute_value |
      | att1           | att_type_1     |                 |
    And a context elements with the before attrs and the following entities
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard entity delete payload with the previous data
    When a standard context entity delete is asked with the before information
    Then retrieve information from the mock
    And there is "0" petitions requested to the mock
    And clean the mongo database of the service "issue_719"
    And print the request and the response

  @issue-719
  Scenario: Entity in CB and in CP with the same subervice, and in other CP with other subservice.
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Append operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice1"
    And the following attributes to create
      | attribute_name | attribute_type | attribute_value |
      | att1           | att_type_1     | 25              |
    And a context elements with the before attrs and the following entities
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard entity creation payload with the previous data
    And a standard context entity creation is asked with the before information
    # Query operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard query context payload with the previous data
    And a standard query context is asked with the before information
    Then retrieve information from the mock
    And there is "0" petitions requested to the mock
    And clean the mongo database of the service "issue_719"
    And print the request and the response

  @issue-719
  Scenario: Entity in CB and in CP with the same subervice, and in other CP with other subservice.
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice1"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Append operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice1"
    And the following attributes to create
      | attribute_name | attribute_type | attribute_value |
      | att1           | att_type_1     | 25              |
    And a context elements with the before attrs and the following entities
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard entity creation payload with the previous data
    And a standard context entity creation is asked with the before information
    # Query operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice2"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard query context payload with the previous data
    And a standard query context is asked with the before information
    Then retrieve information from the mock
    And there is "1" petitions requested to the mock
    And the path in the last mock petition contains "service2"
    And clean the mongo database of the service "issue_719"
    And print the request and the response

  @issue-719
  Scenario: Entity in CB and in CP without servicepath, and in other CP with other servicepath.
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "empty"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Append operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "empty"
    And the following attributes to create
      | attribute_name | attribute_type | attribute_value |
      | att1           | att_type_1     | 25              |
    And a context elements with the before attrs and the following entities
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard entity creation payload with the previous data
    And a standard context entity creation is asked with the before information
    # Query operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard query context payload with the previous data
    And a standard query context is asked with the before information
    Then retrieve information from the mock
    And there is "1" petitions requested to the mock
    And the path in the last mock petition contains "service2"
    And clean the mongo database of the service "issue_719"
    And clean the mongo database of the service ""

  @issue-719
  Scenario: Entity in CB and in CP with servicepath, and in other CP without servicepath.
    Given a started mock
    # First registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "/subservice"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service1 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Second Registration
    And a new NGSI version "9" petition with the service "issue_719" and the subservice "empty"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And the following attributes to consult
      | attribute_name | attribute_type |
      | att1           | att_type_1     |
    And a context registrations with the before entities and attributes and the following providing applications
      | providing_application      |
      | /context_provider/service2 |
    And build the standard context registration payload with the previous data and duration "P1M"
    And a standard context registration is asked with the before information
    # Append operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "/subservice"
    And the following attributes to create
      | attribute_name | attribute_type | attribute_value |
      | att1           | att_type_1     | 25              |
    And a context elements with the before attrs and the following entities
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard entity creation payload with the previous data
    And a standard context entity creation is asked with the before information
    # Query operation
    And a new NGSI version "10" petition with the service "issue_719" and the subservice "empty"
    And the following entities to consult
      | entity_id | entity_type |
      | Room1     | Room        |
    And build the standard query context payload with the previous data
    And a standard query context is asked with the before information
    Then retrieve information from the mock
    And there is "1" petitions requested to the mock
    And the path in the last mock petition contains "service2"
    And clean the mongo database of the service "issue_719"
    And clean the mongo database of the service ""
